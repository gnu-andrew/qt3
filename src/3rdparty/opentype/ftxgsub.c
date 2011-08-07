/*******************************************************************
 *
 *  ftxgsub.c
 *
 *    TrueType Open GSUB table support.
 *
 *  Copyright 1996-2000 by
 *  David Turner, Robert Wilhelm, and Werner Lemberg.
 *
 *  This file is part of the FreeType project, and may only be used
 *  modified and distributed under the terms of the FreeType project
 *  license, LICENSE.TXT.  By continuing to use, modify, or distribute
 *  this file you indicate that you have read the license and
 *  understand and accept it fully.
 *
 ******************************************************************/

/* XXX There is *a lot* of duplicated code (cf. formats 5 and 6), but
       I don't care currently.  I believe that it would be possible to
       save about 50% of TTO code by carefully designing the structures,
       sharing as much as possible with extensive use of macros.  This
       is something for a volunteer :-)                                  */

#define EXPORT_FUNC

#include "ftxopen.h"
#include "ftxopenf.h"

#include "ftglue.h"

#include FT_TRUETYPE_TAGS_H

#define GSUB_ID  Build_Extension_ID( 'G', 'S', 'U', 'B' )


  static FT_Error  GSub_Do_Glyph_Lookup( TTO_GSUBHeader*   gsub,
                                         FT_UShort         lookup_index,
                                         OTL_Buffer        buffer,
                                         FT_UShort         context_length,
                                         int               nesting_level );



  /**********************
   * Auxiliary functions
   **********************/


  EXPORT_FUNC
  FT_Error  TT_Load_GSUB_Table( FT_Face          face,
                                TTO_GSUBHeader** retptr,
                                TTO_GDEFHeader*  gdef )
  {
    FT_Stream        stream = face->stream;
    FT_Memory        memory = face->memory;
    FT_Error         error;
    FT_ULong         cur_offset, new_offset, base_offset;

    FT_UShort        i, num_lookups;
    TTO_GSUBHeader*  gsub;
    TTO_Lookup*      lo;

    if ( !retptr )
      return TT_Err_Invalid_Argument;

    if (( error = ftglue_face_goto_table( face, TTAG_GSUB, stream ) ))
      return error;

    base_offset = FILE_Pos();

    if ( ALLOC ( gsub, sizeof( *gsub ) ) )
      return error;

    gsub->memory = memory;

    /* skip version */

    if ( FILE_Seek( base_offset + 4L ) ||
         ACCESS_Frame( 2L ) )
      goto Fail4;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_ScriptList( &gsub->ScriptList,
                                    stream ) ) != TT_Err_Ok )
      goto Fail4;
    (void)FILE_Seek( cur_offset );

    if ( ACCESS_Frame( 2L ) )
      goto Fail3;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_FeatureList( &gsub->FeatureList,
                                     stream ) ) != TT_Err_Ok )
      goto Fail3;
    (void)FILE_Seek( cur_offset );

    if ( ACCESS_Frame( 2L ) )
      goto Fail2;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_LookupList( &gsub->LookupList,
                                    stream, GSUB ) ) != TT_Err_Ok )
      goto Fail2;

    gsub->gdef = gdef;      /* can be NULL */

    /* We now check the LookupFlags for values larger than 0xFF to find
       out whether we need to load the `MarkAttachClassDef' field of the
       GDEF table -- this hack is necessary for OpenType 1.2 tables since
       the version field of the GDEF table hasn't been incremented.

       For constructed GDEF tables, we only load it if
       `MarkAttachClassDef_offset' is not zero (nevertheless, a build of
       a constructed mark attach table is not supported currently).       */

    if ( gdef &&
         gdef->MarkAttachClassDef_offset && !gdef->MarkAttachClassDef.loaded )
    {
      lo          = gsub->LookupList.Lookup;
      num_lookups = gsub->LookupList.LookupCount;

      for ( i = 0; i < num_lookups; i++ )
      {

        if ( lo[i].LookupFlag & IGNORE_SPECIAL_MARKS )
        {
          if ( FILE_Seek( gdef->MarkAttachClassDef_offset ) ||
               ( error = Load_ClassDefinition( &gdef->MarkAttachClassDef,
                                               256, stream ) ) != TT_Err_Ok )
            goto Fail1;

          break;
        }
      }
    }

    *retptr = gsub;

    return TT_Err_Ok;

  Fail1:
    Free_LookupList( &gsub->LookupList, GSUB, memory );

  Fail2:
    Free_FeatureList( &gsub->FeatureList, memory );

  Fail3:
    Free_ScriptList( &gsub->ScriptList, memory );

  Fail4:
    FREE ( gsub );


    return error;
  }

  EXPORT_FUNC
  FT_Error   TT_Done_GSUB_Table( TTO_GSUBHeader* gsub )
  {
    FT_Memory memory = gsub->memory;
    
    Free_LookupList( &gsub->LookupList, GSUB, memory );
    Free_FeatureList( &gsub->FeatureList, memory );
    Free_ScriptList( &gsub->ScriptList, memory );

    FREE( gsub );

    return TT_Err_Ok;
  }

  /*****************************
   * SubTable related functions
   *****************************/


  /* LookupType 1 */

  /* SingleSubstFormat1 */
  /* SingleSubstFormat2 */

  FT_Error  Load_SingleSubst( TTO_SingleSubst*  ss,
                              FT_Stream         stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort n, count;
    FT_ULong cur_offset, new_offset, base_offset;

    FT_UShort*  s;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 4L ) )
      return error;

    ss->SubstFormat = GET_UShort();
    new_offset      = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_Coverage( &ss->Coverage, stream ) ) != TT_Err_Ok )
      return error;
    (void)FILE_Seek( cur_offset );

    switch ( ss->SubstFormat )
    {
    case 1:
      if ( ACCESS_Frame( 2L ) )
        goto Fail2;

      ss->ssf.ssf1.DeltaGlyphID = GET_UShort();

      FORGET_Frame();

      break;

    case 2:
      if ( ACCESS_Frame( 2L ) )
        goto Fail2;

      count = ss->ssf.ssf2.GlyphCount = GET_UShort();

      FORGET_Frame();

      ss->ssf.ssf2.Substitute = NULL;

      if ( ALLOC_ARRAY( ss->ssf.ssf2.Substitute, count, FT_UShort ) )
        goto Fail2;

      s = ss->ssf.ssf2.Substitute;

      if ( ACCESS_Frame( count * 2L ) )
        goto Fail1;

      for ( n = 0; n < count; n++ )
        s[n] = GET_UShort();

      FORGET_Frame();

      break;

    default:
      return TTO_Err_Invalid_GSUB_SubTable_Format;
    }

    return TT_Err_Ok;

  Fail1:
    FREE( s );

  Fail2:
    Free_Coverage( &ss->Coverage, memory );
    return error;
  }


  void  Free_SingleSubst( TTO_SingleSubst*  ss,
			  FT_Memory         memory )
  {
    switch ( ss->SubstFormat )
    {
    case 1:
      break;

    case 2:
      FREE( ss->ssf.ssf2.Substitute );
      break;
    }

    Free_Coverage( &ss->Coverage, memory );
  }


  static FT_Error  Lookup_SingleSubst( TTO_SingleSubst*  ss,
				       OTL_Buffer        buffer,
                                       FT_UShort         flags,
                                       FT_UShort         context_length,
                                       TTO_GDEFHeader*   gdef )
  {
    FT_UShort index, value, property;
    FT_Error  error;


    if ( context_length != 0xFFFF && context_length < 1 )
      return TTO_Err_Not_Covered;

    if ( CHECK_Property( gdef, IN_CURITEM(), flags, &property ) )
      return error;

    error = Coverage_Index( &ss->Coverage, IN_CURGLYPH(), &index );
    if ( error )
      return error;

    switch ( ss->SubstFormat )
    {
    case 1:
	    value = ( IN_CURGLYPH() + ss->ssf.ssf1.DeltaGlyphID ) & 0xFFFF;
      if ( ADD_Glyph( buffer, value, 0xFFFF, 0xFFFF ) )
        return error;
      break;

    case 2:
      if ( index >= ss->ssf.ssf2.GlyphCount )
        return TTO_Err_Invalid_GSUB_SubTable;
      value = ss->ssf.ssf2.Substitute[index];
      if ( ADD_Glyph( buffer, value, 0xFFFF, 0xFFFF ) )
        return error;
      break;

    default:
      return TTO_Err_Invalid_GSUB_SubTable;
    }

    if ( gdef && gdef->NewGlyphClasses )
    {
      /* we inherit the old glyph class to the substituted glyph */

      error = Add_Glyph_Property( gdef, value, property );
      if ( error && error != TTO_Err_Not_Covered )
        return error;
    }

    return TT_Err_Ok;
  }


  /* LookupType 2 */

  /* Sequence */

  static FT_Error  Load_Sequence( TTO_Sequence*  s,
                                  FT_Stream      stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort n, count;
    FT_UShort*  sub;


    if ( ACCESS_Frame( 2L ) )
      return error;

    count = s->GlyphCount = GET_UShort();

    FORGET_Frame();

    s->Substitute = NULL;

    if ( count )
    {
      if ( ALLOC_ARRAY( s->Substitute, count, FT_UShort ) )
        return error;

      sub = s->Substitute;

      if ( ACCESS_Frame( count * 2L ) )
      {
        FREE( sub );
        return error;
      }

      for ( n = 0; n < count; n++ )
        sub[n] = GET_UShort();

      FORGET_Frame();
    }

    return TT_Err_Ok;
  }


  static void  Free_Sequence( TTO_Sequence*  s,
			      FT_Memory      memory )
  {
    FREE( s->Substitute );
  }


  /* MultipleSubstFormat1 */

  FT_Error  Load_MultipleSubst( TTO_MultipleSubst*  ms,
                                FT_Stream           stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort      n = 0, m, count;
    FT_ULong       cur_offset, new_offset, base_offset;

    TTO_Sequence*  s;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 4L ) )
      return error;

    ms->SubstFormat = GET_UShort();             /* should be 1 */
    new_offset      = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_Coverage( &ms->Coverage, stream ) ) != TT_Err_Ok )
      return error;
    (void)FILE_Seek( cur_offset );

    if ( ACCESS_Frame( 2L ) )
      goto Fail2;

    count = ms->SequenceCount = GET_UShort();

    FORGET_Frame();

    ms->Sequence = NULL;

    if ( ALLOC_ARRAY( ms->Sequence, count, TTO_Sequence ) )
      goto Fail2;

    s = ms->Sequence;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        goto Fail1;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_Sequence( &s[n], stream ) ) != TT_Err_Ok )
        goto Fail1;
      (void)FILE_Seek( cur_offset );
    }

    return TT_Err_Ok;

  Fail1:
    for ( m = 0; m < n; m++ )
      Free_Sequence( &s[m], memory );

    FREE( s );

  Fail2:
    Free_Coverage( &ms->Coverage, memory );
    return error;
  }


  void  Free_MultipleSubst( TTO_MultipleSubst*  ms,
			    FT_Memory           memory )
  {
    FT_UShort      n, count;

    TTO_Sequence*  s;


    if ( ms->Sequence )
    {
      count = ms->SequenceCount;
      s     = ms->Sequence;

      for ( n = 0; n < count; n++ )
        Free_Sequence( &s[n], memory );

      FREE( s );
    }

    Free_Coverage( &ms->Coverage, memory );
  }


  static FT_Error  Lookup_MultipleSubst( TTO_MultipleSubst*  ms,
					 OTL_Buffer          buffer,
                                         FT_UShort           flags,
                                         FT_UShort           context_length,
                                         TTO_GDEFHeader*     gdef )
  {
    FT_Error  error;
    FT_UShort index, property, n, count;
    FT_UShort*s;


    if ( context_length != 0xFFFF && context_length < 1 )
      return TTO_Err_Not_Covered;

    if ( CHECK_Property( gdef, IN_CURITEM(), flags, &property ) )
      return error;

    error = Coverage_Index( &ms->Coverage, IN_CURGLYPH(), &index );
    if ( error )
      return error;

    if ( index >= ms->SequenceCount )
      return TTO_Err_Invalid_GSUB_SubTable;

    count = ms->Sequence[index].GlyphCount;
    s     = ms->Sequence[index].Substitute;

    if ( ADD_String( buffer, 1, count, s, 0xFFFF, 0xFFFF ) )
      return error;

    if ( gdef && gdef->NewGlyphClasses )
    {
      /* this is a guess only ... */

      if ( property == TTO_LIGATURE )
        property = TTO_BASE_GLYPH;

      for ( n = 0; n < count; n++ )
      {
        error = Add_Glyph_Property( gdef, s[n], property );
        if ( error && error != TTO_Err_Not_Covered )
          return error;
      }
    }

    return TT_Err_Ok;
  }


  /* LookupType 3 */

  /* AlternateSet */

  static FT_Error  Load_AlternateSet( TTO_AlternateSet*  as,
                                      FT_Stream          stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort n, count;
    FT_UShort*  a;


    if ( ACCESS_Frame( 2L ) )
      return error;

    count = as->GlyphCount = GET_UShort();

    FORGET_Frame();

    as->Alternate = NULL;

    if ( ALLOC_ARRAY( as->Alternate, count, FT_UShort ) )
      return error;

    a = as->Alternate;

    if ( ACCESS_Frame( count * 2L ) )
    {
      FREE( a );
      return error;
    }

    for ( n = 0; n < count; n++ )
      a[n] = GET_UShort();

    FORGET_Frame();

    return TT_Err_Ok;
  }


  static void  Free_AlternateSet( TTO_AlternateSet*  as,
				  FT_Memory          memory )
  {
    FREE( as->Alternate );
  }


  /* AlternateSubstFormat1 */

  FT_Error  Load_AlternateSubst( TTO_AlternateSubst*  as,
                                 FT_Stream            stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort          n = 0, m, count;
    FT_ULong           cur_offset, new_offset, base_offset;

    TTO_AlternateSet*  aset;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 4L ) )
      return error;

    as->SubstFormat = GET_UShort();             /* should be 1 */
    new_offset      = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_Coverage( &as->Coverage, stream ) ) != TT_Err_Ok )
      return error;
    (void)FILE_Seek( cur_offset );

    if ( ACCESS_Frame( 2L ) )
      goto Fail2;

    count = as->AlternateSetCount = GET_UShort();

    FORGET_Frame();

    as->AlternateSet = NULL;

    if ( ALLOC_ARRAY( as->AlternateSet, count, TTO_AlternateSet ) )
      goto Fail2;

    aset = as->AlternateSet;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        goto Fail1;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_AlternateSet( &aset[n], stream ) ) != TT_Err_Ok )
        goto Fail1;
      (void)FILE_Seek( cur_offset );
    }

    return TT_Err_Ok;

  Fail1:
    for ( m = 0; m < n; m++ )
      Free_AlternateSet( &aset[m], memory );

    FREE( aset );

  Fail2:
    Free_Coverage( &as->Coverage, memory );
    return error;
  }


  void  Free_AlternateSubst( TTO_AlternateSubst*  as,
			     FT_Memory            memory )
  {
    FT_UShort          n, count;

    TTO_AlternateSet*  aset;


    if ( as->AlternateSet )
    {
      count = as->AlternateSetCount;
      aset  = as->AlternateSet;

      for ( n = 0; n < count; n++ )
        Free_AlternateSet( &aset[n], memory );

      FREE( aset );
    }

    Free_Coverage( &as->Coverage, memory );
  }


  static FT_Error  Lookup_AlternateSubst( TTO_GSUBHeader*      gsub,
                                          TTO_AlternateSubst*  as,
					  OTL_Buffer           buffer,
                                          FT_UShort            flags,
                                          FT_UShort            context_length,
                                          TTO_GDEFHeader*      gdef )
  {
    FT_Error          error;
    FT_UShort         index, alt_index, property;

    TTO_AlternateSet  aset;


    if ( context_length != 0xFFFF && context_length < 1 )
      return TTO_Err_Not_Covered;

    if ( CHECK_Property( gdef, IN_CURITEM(), flags, &property ) )
      return error;

    error = Coverage_Index( &as->Coverage, IN_CURGLYPH(), &index );
    if ( error )
      return error;

    aset = as->AlternateSet[index];

    /* we use a user-defined callback function to get the alternate index */

    if ( gsub->altfunc )
      alt_index = (gsub->altfunc)( buffer->out_pos, IN_CURGLYPH(),
                                   aset.GlyphCount, aset.Alternate,
                                   gsub->data );
    else
      alt_index = 0;

    if ( ADD_Glyph( buffer, aset.Alternate[alt_index],
		    0xFFFF, 0xFFFF ) )
      return error;

    if ( gdef && gdef->NewGlyphClasses )
    {
      /* we inherit the old glyph class to the substituted glyph */

      error = Add_Glyph_Property( gdef, aset.Alternate[alt_index],
                                  property );
      if ( error && error != TTO_Err_Not_Covered )
        return error;
    }

    return TT_Err_Ok;
  }


  /* LookupType 4 */

  /* Ligature */

  static FT_Error  Load_Ligature( TTO_Ligature*  l,
                                  FT_Stream      stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort n, count;
    FT_UShort*  c;


    if ( ACCESS_Frame( 4L ) )
      return error;

    l->LigGlyph       = GET_UShort();
    l->ComponentCount = GET_UShort();

    FORGET_Frame();

    l->Component = NULL;

    count = l->ComponentCount - 1;      /* only ComponentCount - 1 elements */

    if ( ALLOC_ARRAY( l->Component, count, FT_UShort ) )
      return error;

    c = l->Component;

    if ( ACCESS_Frame( count * 2L ) )
    {
      FREE( c );
      return error;
    }

    for ( n = 0; n < count; n++ )
      c[n] = GET_UShort();

    FORGET_Frame();

    return TT_Err_Ok;
  }


  static void  Free_Ligature( TTO_Ligature*  l,
			      FT_Memory      memory )
  {
    FREE( l->Component );
  }


  /* LigatureSet */

  static FT_Error  Load_LigatureSet( TTO_LigatureSet*  ls,
                                     FT_Stream         stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort      n = 0, m, count;
    FT_ULong       cur_offset, new_offset, base_offset;

    TTO_Ligature*  l;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 2L ) )
      return error;

    count = ls->LigatureCount = GET_UShort();

    FORGET_Frame();

    ls->Ligature = NULL;

    if ( ALLOC_ARRAY( ls->Ligature, count, TTO_Ligature ) )
      return error;

    l = ls->Ligature;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        goto Fail;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_Ligature( &l[n], stream ) ) != TT_Err_Ok )
        goto Fail;
      (void)FILE_Seek( cur_offset );
    }

    return TT_Err_Ok;

  Fail:
    for ( m = 0; m < n; m++ )
      Free_Ligature( &l[m], memory );

    FREE( l );
    return error;
  }


  static void  Free_LigatureSet( TTO_LigatureSet*  ls,
				 FT_Memory         memory )
  {
    FT_UShort      n, count;

    TTO_Ligature*  l;


    if ( ls->Ligature )
    {
      count = ls->LigatureCount;
      l     = ls->Ligature;

      for ( n = 0; n < count; n++ )
        Free_Ligature( &l[n], memory );

      FREE( l );
    }
  }


  /* LigatureSubstFormat1 */

  FT_Error  Load_LigatureSubst( TTO_LigatureSubst*  ls,
                                FT_Stream           stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort         n = 0, m, count;
    FT_ULong          cur_offset, new_offset, base_offset;

    TTO_LigatureSet*  lset;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 4L ) )
      return error;

    ls->SubstFormat = GET_UShort();             /* should be 1 */
    new_offset      = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_Coverage( &ls->Coverage, stream ) ) != TT_Err_Ok )
      return error;
    (void)FILE_Seek( cur_offset );

    if ( ACCESS_Frame( 2L ) )
      goto Fail2;

    count = ls->LigatureSetCount = GET_UShort();

    FORGET_Frame();

    ls->LigatureSet = NULL;

    if ( ALLOC_ARRAY( ls->LigatureSet, count, TTO_LigatureSet ) )
      goto Fail2;

    lset = ls->LigatureSet;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        goto Fail1;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_LigatureSet( &lset[n], stream ) ) != TT_Err_Ok )
        goto Fail1;
      (void)FILE_Seek( cur_offset );
    }

    return TT_Err_Ok;

  Fail1:
    for ( m = 0; m < n; m++ )
      Free_LigatureSet( &lset[m], memory );

    FREE( lset );

  Fail2:
    Free_Coverage( &ls->Coverage, memory );
    return error;
  }


  void  Free_LigatureSubst( TTO_LigatureSubst*  ls,
			    FT_Memory           memory )
  {
    FT_UShort         n, count;

    TTO_LigatureSet*  lset;


    if ( ls->LigatureSet )
    {
      count = ls->LigatureSetCount;
      lset  = ls->LigatureSet;

      for ( n = 0; n < count; n++ )
        Free_LigatureSet( &lset[n], memory );

      FREE( lset );
    }

    Free_Coverage( &ls->Coverage, memory );
  }


  static FT_Error  Lookup_LigatureSubst( TTO_LigatureSubst*  ls,
					 OTL_Buffer          buffer,
                                         FT_UShort           flags,
                                         FT_UShort           context_length,
                                         TTO_GDEFHeader*     gdef )
  {
    FT_UShort      index, property;
    FT_Error       error;
    FT_UShort      numlig, i, j, is_mark, first_is_mark = FALSE;
    FT_UShort*     c;

    TTO_Ligature*  lig;


    if ( CHECK_Property( gdef, IN_CURITEM(), flags, &property ) )
      return error;

    if ( property == TTO_MARK || property & IGNORE_SPECIAL_MARKS )
      first_is_mark = TRUE;

    error = Coverage_Index( &ls->Coverage, IN_CURGLYPH(), &index );
    if ( error )
      return error;

    if ( index >= ls->LigatureSetCount )
       return TTO_Err_Invalid_GSUB_SubTable;

    lig = ls->LigatureSet[index].Ligature;

    for ( numlig = ls->LigatureSet[index].LigatureCount;
          numlig;
          numlig--, lig++ )
    {
      if ( buffer->in_pos + lig->ComponentCount > buffer->in_length )
        goto next_ligature;               /* Not enough glyphs in input */

      c    = lig->Component;

      is_mark = first_is_mark;

      if ( context_length != 0xFFFF && context_length < lig->ComponentCount )
        break;

      for ( i = 1, j = buffer->in_pos + 1; i < lig->ComponentCount; i++, j++ )
      {
        while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
        {
          if ( error && error != TTO_Err_Not_Covered )
            return error;

          if ( j + lig->ComponentCount - i == (FT_Int)buffer->in_length )
	    goto next_ligature;
	  j++;
        }

        if ( !( property == TTO_MARK || property & IGNORE_SPECIAL_MARKS ) )
          is_mark = FALSE;

        if ( IN_GLYPH( j ) != c[i - 1] )
	  goto next_ligature;
      }

      if ( gdef && gdef->NewGlyphClasses )
      {
	/* this is just a guess ... */

	error = Add_Glyph_Property( gdef, lig->LigGlyph,
				    is_mark ? TTO_MARK : TTO_LIGATURE );
	if ( error && error != TTO_Err_Not_Covered )
	  return error;
      }

      if ( j == buffer->in_pos + i ) /* No input glyphs skipped */
      {
	/* We don't use a new ligature ID if there are no skipped
	   glyphs and the ligature already has an ID.             */

	if ( IN_LIGID( buffer->in_pos ) )
	{
	  if ( ADD_String( buffer, i, 1, &lig->LigGlyph,
			  0xFFFF, 0xFFFF ) )
	    return error;
	}
	else
	{
	  FT_UShort ligID = otl_buffer_allocate_ligid( buffer );
	  if ( ADD_String( buffer, i, 1, &lig->LigGlyph,
			  0xFFFF, ligID ) )
	    return error;
	}
      }
      else
      {
	FT_UShort ligID = otl_buffer_allocate_ligid( buffer );
	if ( ADD_Glyph( buffer, lig->LigGlyph,
			0xFFFF, ligID ) )
	  return error;

	/* Now we must do a second loop to copy the skipped glyphs to
	   `out' and assign component values to it.  We start with the
	   glyph after the first component.  Glyphs between component
	   i and i+1 belong to component i.  Together with the ligID
	   value it is later possible to check whether a specific
	   component value really belongs to a given ligature.         */

	for ( i = 0; i < lig->ComponentCount - 1; i++ )
	{
	  while ( CHECK_Property( gdef, IN_CURITEM(),
				  flags, &property ) )
	    if ( ADD_Glyph( buffer, IN_CURGLYPH(),
			    i, ligID ) )
	      return error;

	  (buffer->in_pos)++;
	}
      }

      return TT_Err_Ok;

    next_ligature:
      ;
    }

    return TTO_Err_Not_Covered;
  }


  /* Do the actual substitution for a context substitution (either format
     5 or 6).  This is only called after we've determined that the input
     matches the subrule.                                                 */

  static FT_Error  Do_ContextSubst( TTO_GSUBHeader*         gsub,
                                    FT_UShort               GlyphCount,
                                    FT_UShort               SubstCount,
                                    TTO_SubstLookupRecord*  subst,
				    OTL_Buffer              buffer,
                                    int                     nesting_level )
  {
    FT_Error  error;
    FT_UShort i, old_pos;


    i = 0;

    while ( i < GlyphCount )
    {
      if ( SubstCount && i == subst->SequenceIndex )
      {
        old_pos = buffer->in_pos;

        /* Do a substitution */

        error = GSub_Do_Glyph_Lookup( gsub, subst->LookupListIndex, buffer,
                                      GlyphCount, nesting_level );

        subst++;
        SubstCount--;
        i += buffer->in_pos - old_pos;

        if ( error == TTO_Err_Not_Covered )
        {
          /* XXX "can't happen" -- but don't count on it */

          if ( ADD_Glyph( buffer, IN_CURGLYPH(),
			  0xFFFF, 0xFFFF ) )
            return error;
          i++;
        }
        else if ( error )
          return error;
      }
      else
      {
        /* No substitution for this index */

        if ( ADD_Glyph( buffer, IN_CURGLYPH(),
			0xFFFF, 0xFFFF ) )
          return error;
        i++;
      }
    }

    return TT_Err_Ok;
  }


  /* LookupType 5 */

  /* SubRule */

  static FT_Error  Load_SubRule( TTO_SubRule*  sr,
                                 FT_Stream     stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort               n, count;
    FT_UShort*              i;

    TTO_SubstLookupRecord*  slr;


    if ( ACCESS_Frame( 4L ) )
      return error;

    sr->GlyphCount = GET_UShort();
    sr->SubstCount = GET_UShort();

    FORGET_Frame();

    sr->Input = NULL;

    count = sr->GlyphCount - 1;         /* only GlyphCount - 1 elements */

    if ( ALLOC_ARRAY( sr->Input, count, FT_UShort ) )
      return error;

    i = sr->Input;

    if ( ACCESS_Frame( count * 2L ) )
      goto Fail2;

    for ( n = 0; n < count; n++ )
      i[n] = GET_UShort();

    FORGET_Frame();

    sr->SubstLookupRecord = NULL;

    count = sr->SubstCount;

    if ( ALLOC_ARRAY( sr->SubstLookupRecord, count, TTO_SubstLookupRecord ) )
      goto Fail2;

    slr = sr->SubstLookupRecord;

    if ( ACCESS_Frame( count * 4L ) )
      goto Fail1;

    for ( n = 0; n < count; n++ )
    {
      slr[n].SequenceIndex   = GET_UShort();
      slr[n].LookupListIndex = GET_UShort();
    }

    FORGET_Frame();

    return TT_Err_Ok;

  Fail1:
    FREE( slr );

  Fail2:
    FREE( i );
    return error;
  }


  static void  Free_SubRule( TTO_SubRule*  sr,
			     FT_Memory     memory )
  {
    FREE( sr->SubstLookupRecord );
    FREE( sr->Input );
  }


  /* SubRuleSet */

  static FT_Error  Load_SubRuleSet( TTO_SubRuleSet*  srs,
                                    FT_Stream        stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort     n = 0, m, count;
    FT_ULong      cur_offset, new_offset, base_offset;

    TTO_SubRule*  sr;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 2L ) )
      return error;

    count = srs->SubRuleCount = GET_UShort();

    FORGET_Frame();

    srs->SubRule = NULL;

    if ( ALLOC_ARRAY( srs->SubRule, count, TTO_SubRule ) )
      return error;

    sr = srs->SubRule;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        goto Fail;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_SubRule( &sr[n], stream ) ) != TT_Err_Ok )
        goto Fail;
      (void)FILE_Seek( cur_offset );
    }

    return TT_Err_Ok;

  Fail:
    for ( m = 0; m < n; m++ )
      Free_SubRule( &sr[m], memory );

    FREE( sr );
    return error;
  }


  static void  Free_SubRuleSet( TTO_SubRuleSet*  srs,
				FT_Memory        memory )
  {
    FT_UShort     n, count;

    TTO_SubRule*  sr;


    if ( srs->SubRule )
    {
      count = srs->SubRuleCount;
      sr    = srs->SubRule;

      for ( n = 0; n < count; n++ )
        Free_SubRule( &sr[n], memory );

      FREE( sr );
    }
  }


  /* ContextSubstFormat1 */

  static FT_Error  Load_ContextSubst1( TTO_ContextSubstFormat1*  csf1,
                                       FT_Stream                 stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort        n = 0, m, count;
    FT_ULong         cur_offset, new_offset, base_offset;

    TTO_SubRuleSet*  srs;


    base_offset = FILE_Pos() - 2L;

    if ( ACCESS_Frame( 2L ) )
      return error;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_Coverage( &csf1->Coverage, stream ) ) != TT_Err_Ok )
      return error;
    (void)FILE_Seek( cur_offset );

    if ( ACCESS_Frame( 2L ) )
      goto Fail2;

    count = csf1->SubRuleSetCount = GET_UShort();

    FORGET_Frame();

    csf1->SubRuleSet = NULL;

    if ( ALLOC_ARRAY( csf1->SubRuleSet, count, TTO_SubRuleSet ) )
      goto Fail2;

    srs = csf1->SubRuleSet;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        goto Fail1;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_SubRuleSet( &srs[n], stream ) ) != TT_Err_Ok )
        goto Fail1;
      (void)FILE_Seek( cur_offset );
    }

    return TT_Err_Ok;

  Fail1:
    for ( m = 0; m < n; m++ )
      Free_SubRuleSet( &srs[m], memory );

    FREE( srs );

  Fail2:
    Free_Coverage( &csf1->Coverage, memory );
    return error;
  }


  static void  Gsub_Free_Context1( TTO_ContextSubstFormat1* csf1,
			      FT_Memory                memory )
  {
    FT_UShort        n, count;

    TTO_SubRuleSet*  srs;


    if ( csf1->SubRuleSet )
    {
      count = csf1->SubRuleSetCount;
      srs   = csf1->SubRuleSet;

      for ( n = 0; n < count; n++ )
        Free_SubRuleSet( &srs[n], memory );

      FREE( srs );
    }

    Free_Coverage( &csf1->Coverage, memory );
  }


  /* SubClassRule */

  static FT_Error  Load_SubClassRule( TTO_ContextSubstFormat2*  csf2,
                                      TTO_SubClassRule*         scr,
                                      FT_Stream                 stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort               n, count;

    FT_UShort*              c;
    TTO_SubstLookupRecord*  slr;
    FT_Bool*                d;


    if ( ACCESS_Frame( 4L ) )
      return error;

    scr->GlyphCount = GET_UShort();
    scr->SubstCount = GET_UShort();

    if ( scr->GlyphCount > csf2->MaxContextLength )
      csf2->MaxContextLength = scr->GlyphCount;

    FORGET_Frame();

    scr->Class = NULL;

    count = scr->GlyphCount - 1;        /* only GlyphCount - 1 elements */

    if ( ALLOC_ARRAY( scr->Class, count, FT_UShort ) )
      return error;

    c = scr->Class;
    d = csf2->ClassDef.Defined;

    if ( ACCESS_Frame( count * 2L ) )
      goto Fail2;

    for ( n = 0; n < count; n++ )
    {
      c[n] = GET_UShort();

      /* We check whether the specific class is used at all.  If not,
         class 0 is used instead.                                     */
      if ( !d[c[n]] )
        c[n] = 0;
    }

    FORGET_Frame();

    scr->SubstLookupRecord = NULL;

    count = scr->SubstCount;

    if ( ALLOC_ARRAY( scr->SubstLookupRecord, count, TTO_SubstLookupRecord ) )
      goto Fail2;

    slr = scr->SubstLookupRecord;

    if ( ACCESS_Frame( count * 4L ) )
      goto Fail1;

    for ( n = 0; n < count; n++ )
    {
      slr[n].SequenceIndex   = GET_UShort();
      slr[n].LookupListIndex = GET_UShort();
    }

    FORGET_Frame();

    return TT_Err_Ok;

  Fail1:
    FREE( slr );

  Fail2:
    FREE( c );
    return error;
  }


  static void  Free_SubClassRule( TTO_SubClassRule*  scr,
				  FT_Memory          memory )
  {
    FREE( scr->SubstLookupRecord );
    FREE( scr->Class );
  }


  /* SubClassSet */

  static FT_Error  Load_SubClassSet( TTO_ContextSubstFormat2*  csf2,
                                     TTO_SubClassSet*          scs,
                                     FT_Stream                 stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort          n = 0, m, count;
    FT_ULong           cur_offset, new_offset, base_offset;

    TTO_SubClassRule*  scr;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 2L ) )
      return error;

    count = scs->SubClassRuleCount = GET_UShort();

    FORGET_Frame();

    scs->SubClassRule = NULL;

    if ( ALLOC_ARRAY( scs->SubClassRule, count, TTO_SubClassRule ) )
      return error;

    scr = scs->SubClassRule;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        goto Fail;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_SubClassRule( csf2, &scr[n],
                                        stream ) ) != TT_Err_Ok )
        goto Fail;
      (void)FILE_Seek( cur_offset );
    }

    return TT_Err_Ok;

  Fail:
    for ( m = 0; m < n; m++ )
      Free_SubClassRule( &scr[m], memory );

    FREE( scr );
    return error;
  }


  static void  Free_SubClassSet( TTO_SubClassSet*  scs,
				 FT_Memory         memory )
  {
    FT_UShort          n, count;

    TTO_SubClassRule*  scr;


    if ( scs->SubClassRule )
    {
      count = scs->SubClassRuleCount;
      scr   = scs->SubClassRule;

      for ( n = 0; n < count; n++ )
        Free_SubClassRule( &scr[n], memory );

      FREE( scr );
    }
  }


  /* ContextSubstFormat2 */

  static FT_Error  Load_ContextSubst2( TTO_ContextSubstFormat2*  csf2,
                                       FT_Stream                 stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort         n = 0, m, count;
    FT_ULong          cur_offset, new_offset, base_offset;

    TTO_SubClassSet*  scs;


    base_offset = FILE_Pos() - 2;

    if ( ACCESS_Frame( 2L ) )
      return error;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_Coverage( &csf2->Coverage, stream ) ) != TT_Err_Ok )
      return error;
    (void)FILE_Seek( cur_offset );

    if ( ACCESS_Frame( 4L ) )
      goto Fail3;

    new_offset = GET_UShort() + base_offset;

    /* `SubClassSetCount' is the upper limit for class values, thus we
       read it now to make an additional safety check.                 */

    count = csf2->SubClassSetCount = GET_UShort();

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_ClassDefinition( &csf2->ClassDef, count,
                                         stream ) ) != TT_Err_Ok )
      goto Fail3;
    (void)FILE_Seek( cur_offset );

    csf2->SubClassSet      = NULL;
    csf2->MaxContextLength = 0;

    if ( ALLOC_ARRAY( csf2->SubClassSet, count, TTO_SubClassSet ) )
      goto Fail2;

    scs = csf2->SubClassSet;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        goto Fail1;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      if ( new_offset != base_offset )      /* not a NULL offset */
      {
        cur_offset = FILE_Pos();
        if ( FILE_Seek( new_offset ) ||
             ( error = Load_SubClassSet( csf2, &scs[n],
                                         stream ) ) != TT_Err_Ok )
          goto Fail1;
        (void)FILE_Seek( cur_offset );
      }
      else
      {
        /* we create a SubClassSet table with no entries */

        csf2->SubClassSet[n].SubClassRuleCount = 0;
        csf2->SubClassSet[n].SubClassRule      = NULL;
      }
    }

    return TT_Err_Ok;

  Fail1:
    for ( m = 0; m < n; m++ )
      Free_SubClassSet( &scs[m], memory );

    FREE( scs );

  Fail2:
    Free_ClassDefinition( &csf2->ClassDef, memory );

  Fail3:
    Free_Coverage( &csf2->Coverage, memory );
    return error;
  }


  static void  Gsub_Free_Context2( TTO_ContextSubstFormat2*  csf2,
			      FT_Memory                 memory )
  {
    FT_UShort         n, count;

    TTO_SubClassSet*  scs;


    if ( csf2->SubClassSet )
    {
      count = csf2->SubClassSetCount;
      scs   = csf2->SubClassSet;

      for ( n = 0; n < count; n++ )
        Free_SubClassSet( &scs[n], memory );

      FREE( scs );
    }

    Free_ClassDefinition( &csf2->ClassDef, memory );
    Free_Coverage( &csf2->Coverage, memory );
  }


  /* ContextSubstFormat3 */

  static FT_Error  Load_ContextSubst3( TTO_ContextSubstFormat3*  csf3,
                                       FT_Stream                 stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort               n = 0, m, count;
    FT_ULong                cur_offset, new_offset, base_offset;

    TTO_Coverage*           c;
    TTO_SubstLookupRecord*  slr;


    base_offset = FILE_Pos() - 2L;

    if ( ACCESS_Frame( 4L ) )
      return error;

    csf3->GlyphCount = GET_UShort();
    csf3->SubstCount = GET_UShort();

    FORGET_Frame();

    csf3->Coverage = NULL;

    count = csf3->GlyphCount;

    if ( ALLOC_ARRAY( csf3->Coverage, count, TTO_Coverage ) )
      return error;

    c = csf3->Coverage;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        goto Fail2;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_Coverage( &c[n], stream ) ) != TT_Err_Ok )
        goto Fail2;
      (void)FILE_Seek( cur_offset );
    }

    csf3->SubstLookupRecord = NULL;

    count = csf3->SubstCount;

    if ( ALLOC_ARRAY( csf3->SubstLookupRecord, count,
                      TTO_SubstLookupRecord ) )
      goto Fail2;

    slr = csf3->SubstLookupRecord;

    if ( ACCESS_Frame( count * 4L ) )
      goto Fail1;

    for ( n = 0; n < count; n++ )
    {
      slr[n].SequenceIndex   = GET_UShort();
      slr[n].LookupListIndex = GET_UShort();
    }

    FORGET_Frame();

    return TT_Err_Ok;

  Fail1:
    FREE( slr );

  Fail2:
    for ( m = 0; m < n; m++ )
      Free_Coverage( &c[m], memory );

    FREE( c );
    return error;
  }


  static void  Gsub_Free_Context3( TTO_ContextSubstFormat3*  csf3,
			      FT_Memory                 memory )
  {
    FT_UShort      n, count;

    TTO_Coverage*  c;


    FREE( csf3->SubstLookupRecord );

    if ( csf3->Coverage )
    {
      count = csf3->GlyphCount;
      c     = csf3->Coverage;

      for ( n = 0; n < count; n++ )
        Free_Coverage( &c[n], memory );

      FREE( c );
    }
  }


  /* ContextSubst */

  FT_Error  Load_ContextSubst( TTO_ContextSubst*  cs,
                               FT_Stream          stream )
  {
    FT_Error error;


    if ( ACCESS_Frame( 2L ) )
      return error;

    cs->SubstFormat = GET_UShort();

    FORGET_Frame();

    switch ( cs->SubstFormat )
    {
    case 1:
      return Load_ContextSubst1( &cs->csf.csf1, stream );

    case 2:
      return Load_ContextSubst2( &cs->csf.csf2, stream );

    case 3:
      return Load_ContextSubst3( &cs->csf.csf3, stream );

    default:
      return TTO_Err_Invalid_GSUB_SubTable_Format;
    }

    return TT_Err_Ok;               /* never reached */
  }


  void  Free_ContextSubst( TTO_ContextSubst*  cs,
			   FT_Memory          memory )
  {
    switch ( cs->SubstFormat )
    {
    case 1:
      Gsub_Free_Context1( &cs->csf.csf1, memory );
      break;

    case 2:
      Gsub_Free_Context2( &cs->csf.csf2, memory );
      break;

    case 3:
      Gsub_Free_Context3( &cs->csf.csf3, memory );
      break;
    }
  }


  static FT_Error  Lookup_ContextSubst1(
                     TTO_GSUBHeader*           gsub,
                     TTO_ContextSubstFormat1*  csf1,
		     OTL_Buffer                buffer,
                     FT_UShort                 flags,
                     FT_UShort                 context_length,
                     int                       nesting_level )
  {
    FT_UShort        index, property;
    FT_UShort        i, j, k, numsr;
    FT_Error         error;

    TTO_SubRule*     sr;
    TTO_GDEFHeader*  gdef;


    gdef = gsub->gdef;

    if ( CHECK_Property( gdef, IN_CURITEM(), flags, &property ) )
      return error;

    error = Coverage_Index( &csf1->Coverage, IN_CURGLYPH(), &index );
    if ( error )
      return error;

    sr    = csf1->SubRuleSet[index].SubRule;
    numsr = csf1->SubRuleSet[index].SubRuleCount;

    for ( k = 0; k < numsr; k++ )
    {
      if ( context_length != 0xFFFF && context_length < sr[k].GlyphCount )
        goto next_subrule;

      if ( buffer->in_pos + sr[k].GlyphCount > buffer->in_length )
        goto next_subrule;                        /* context is too long */

      for ( i = 1, j = buffer->in_pos + 1; i < sr[k].GlyphCount; i++, j++ )
      {
        while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
        {
          if ( error && error != TTO_Err_Not_Covered )
            return error;

          if ( j + sr[k].GlyphCount - i == (FT_Int)buffer->in_length )
	    goto next_subrule;
	  j++;
        }

        if ( IN_GLYPH( j ) != sr[k].Input[i - 1] )
	  goto next_subrule;
      }

      return Do_ContextSubst( gsub, sr[k].GlyphCount,
			      sr[k].SubstCount, sr[k].SubstLookupRecord,
			      buffer,
			      nesting_level );
    next_subrule:
      ;
    }

    return TTO_Err_Not_Covered;
  }


  static FT_Error  Lookup_ContextSubst2(
                     TTO_GSUBHeader*           gsub,
                     TTO_ContextSubstFormat2*  csf2,
		     OTL_Buffer                buffer,
                     FT_UShort                 flags,
                     FT_UShort                 context_length,
                     int                       nesting_level )
  {
    FT_UShort          index, property;
    FT_Error           error;
    FT_Memory          memory = gsub->memory;
    FT_UShort          i, j, k, known_classes;

    FT_UShort*         classes;
    FT_UShort*         cl;

    TTO_SubClassSet*   scs;
    TTO_SubClassRule*  sr;
    TTO_GDEFHeader*    gdef;


    gdef = gsub->gdef;

    if ( CHECK_Property( gdef, IN_CURITEM(), flags, &property ) )
      return error;

    /* Note: The coverage table in format 2 doesn't give an index into
             anything.  It just lets us know whether or not we need to
             do any lookup at all.                                     */

    error = Coverage_Index( &csf2->Coverage, IN_CURGLYPH(), &index );
    if ( error )
      return error;

    if ( ALLOC_ARRAY( classes, csf2->MaxContextLength, FT_UShort ) )
      return error;

    error = Get_Class( &csf2->ClassDef, IN_CURGLYPH(),
                       &classes[0], NULL );
    if ( error && error != TTO_Err_Not_Covered )
      goto End;
    known_classes = 0;

    scs = &csf2->SubClassSet[classes[0]];
    if ( !scs )
    {
      error = TTO_Err_Invalid_GSUB_SubTable;
      goto End;
    }

    for ( k = 0; k < scs->SubClassRuleCount; k++ )
    {
      sr  = &scs->SubClassRule[k];

      if ( context_length != 0xFFFF && context_length < sr->GlyphCount )
        goto next_subclassrule;

      if ( buffer->in_pos + sr->GlyphCount > buffer->in_length )
        goto next_subclassrule;                      /* context is too long */

      cl   = sr->Class;

      /* Start at 1 because [0] is implied */

      for ( i = 1, j = buffer->in_pos + 1; i < sr->GlyphCount; i++, j++ )
      {
        while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
        {
          if ( error && error != TTO_Err_Not_Covered )
            goto End;

          if ( j + sr->GlyphCount - i < (FT_Int)buffer->in_length )
	    goto next_subclassrule;
	  j++;
        }

        if ( i > known_classes )
        {
          /* Keeps us from having to do this for each rule */

          error = Get_Class( &csf2->ClassDef, IN_GLYPH( j ), &classes[i], NULL );
          if ( error && error != TTO_Err_Not_Covered )
            goto End;
          known_classes = i;
        }

        if ( cl[i - 1] != classes[i] )
          goto next_subclassrule;
      }

      error = Do_ContextSubst( gsub, sr->GlyphCount,
			       sr->SubstCount, sr->SubstLookupRecord,
			       buffer,
			       nesting_level );
      goto End;

    next_subclassrule:
      ;
    }

    error = TTO_Err_Not_Covered;

  End:
    FREE( classes );
    return error;
  }


  static FT_Error  Lookup_ContextSubst3(
                     TTO_GSUBHeader*           gsub,
                     TTO_ContextSubstFormat3*  csf3,
		     OTL_Buffer                buffer,
                     FT_UShort                 flags,
                     FT_UShort                 context_length,
                     int                       nesting_level )
  {
    FT_Error         error;
    FT_UShort        index, i, j, property;

    TTO_Coverage*    c;
    TTO_GDEFHeader*  gdef;


    gdef = gsub->gdef;

    if ( CHECK_Property( gdef, IN_CURITEM(), flags, &property ) )
      return error;

    if ( context_length != 0xFFFF && context_length < csf3->GlyphCount )
      return TTO_Err_Not_Covered;

    if ( buffer->in_pos + csf3->GlyphCount > buffer->in_length )
      return TTO_Err_Not_Covered;         /* context is too long */

    c    = csf3->Coverage;

    for ( i = 1, j = buffer->in_pos + 1; i < csf3->GlyphCount; i++, j++ )
    {
      while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
      {
        if ( error && error != TTO_Err_Not_Covered )
          return error;

	if ( j + csf3->GlyphCount - i == (FT_Int)buffer->in_length )
          return TTO_Err_Not_Covered;
	j++;
      }

      error = Coverage_Index( &c[i], IN_GLYPH( j ), &index );
      if ( error )
        return error;
    }

    return Do_ContextSubst( gsub, csf3->GlyphCount,
                            csf3->SubstCount, csf3->SubstLookupRecord,
			    buffer,
                            nesting_level );
  }


  static FT_Error  Lookup_ContextSubst( TTO_GSUBHeader*    gsub,
                                        TTO_ContextSubst*  cs,
					OTL_Buffer         buffer,
                                        FT_UShort          flags,
                                        FT_UShort          context_length,
                                        int                nesting_level )
  {
    switch ( cs->SubstFormat )
    {
    case 1:
      return Lookup_ContextSubst1( gsub, &cs->csf.csf1, buffer,
                                   flags, context_length, nesting_level );

    case 2:
      return Lookup_ContextSubst2( gsub, &cs->csf.csf2, buffer,
                                   flags, context_length, nesting_level );

    case 3:
      return Lookup_ContextSubst3( gsub, &cs->csf.csf3, buffer,
                                   flags, context_length, nesting_level );

    default:
      return TTO_Err_Invalid_GSUB_SubTable_Format;
    }

    return TT_Err_Ok;               /* never reached */
  }


  /* LookupType 6 */

  /* ChainSubRule */

  static FT_Error  Load_ChainSubRule( TTO_ChainSubRule*  csr,
                                      FT_Stream          stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort               n, count;
    FT_UShort*              b;
    FT_UShort*              i;
    FT_UShort*              l;

    TTO_SubstLookupRecord*  slr;


    if ( ACCESS_Frame( 2L ) )
      return error;

    csr->BacktrackGlyphCount = GET_UShort();

    FORGET_Frame();

    csr->Backtrack = NULL;

    count = csr->BacktrackGlyphCount;

    if ( ALLOC_ARRAY( csr->Backtrack, count, FT_UShort ) )
      return error;

    b = csr->Backtrack;

    if ( ACCESS_Frame( count * 2L ) )
      goto Fail4;

    for ( n = 0; n < count; n++ )
      b[n] = GET_UShort();

    FORGET_Frame();

    if ( ACCESS_Frame( 2L ) )
      goto Fail4;

    csr->InputGlyphCount = GET_UShort();

    FORGET_Frame();

    csr->Input = NULL;

    count = csr->InputGlyphCount - 1;  /* only InputGlyphCount - 1 elements */

    if ( ALLOC_ARRAY( csr->Input, count, FT_UShort ) )
      goto Fail4;

    i = csr->Input;

    if ( ACCESS_Frame( count * 2L ) )
      goto Fail3;

    for ( n = 0; n < count; n++ )
      i[n] = GET_UShort();

    FORGET_Frame();

    if ( ACCESS_Frame( 2L ) )
      goto Fail3;

    csr->LookaheadGlyphCount = GET_UShort();

    FORGET_Frame();

    csr->Lookahead = NULL;

    count = csr->LookaheadGlyphCount;

    if ( ALLOC_ARRAY( csr->Lookahead, count, FT_UShort ) )
      goto Fail3;

    l = csr->Lookahead;

    if ( ACCESS_Frame( count * 2L ) )
      goto Fail2;

    for ( n = 0; n < count; n++ )
      l[n] = GET_UShort();

    FORGET_Frame();

    if ( ACCESS_Frame( 2L ) )
      goto Fail2;

    csr->SubstCount = GET_UShort();

    FORGET_Frame();

    csr->SubstLookupRecord = NULL;

    count = csr->SubstCount;

    if ( ALLOC_ARRAY( csr->SubstLookupRecord, count, TTO_SubstLookupRecord ) )
      goto Fail2;

    slr = csr->SubstLookupRecord;

    if ( ACCESS_Frame( count * 4L ) )
      goto Fail1;

    for ( n = 0; n < count; n++ )
    {
      slr[n].SequenceIndex   = GET_UShort();
      slr[n].LookupListIndex = GET_UShort();
    }

    FORGET_Frame();

    return TT_Err_Ok;

  Fail1:
    FREE( slr );

  Fail2:
    FREE( l );

  Fail3:
    FREE( i );

  Fail4:
    FREE( b );
    return error;
  }


  static void  Gsub_Free_ChainSubRule( TTO_ChainSubRule*  csr,
				  FT_Memory          memory )
  {
    FREE( csr->SubstLookupRecord );
    FREE( csr->Lookahead );
    FREE( csr->Input );
    FREE( csr->Backtrack );
  }


  /* ChainSubRuleSet */

  static FT_Error  Load_ChainSubRuleSet( TTO_ChainSubRuleSet*  csrs,
                                         FT_Stream             stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort          n = 0, m, count;
    FT_ULong           cur_offset, new_offset, base_offset;

    TTO_ChainSubRule*  csr;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 2L ) )
      return error;

    count = csrs->ChainSubRuleCount = GET_UShort();

    FORGET_Frame();

    csrs->ChainSubRule = NULL;

    if ( ALLOC_ARRAY( csrs->ChainSubRule, count, TTO_ChainSubRule ) )
      return error;

    csr = csrs->ChainSubRule;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        goto Fail;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_ChainSubRule( &csr[n], stream ) ) != TT_Err_Ok )
        goto Fail;
      (void)FILE_Seek( cur_offset );
    }

    return TT_Err_Ok;

  Fail:
    for ( m = 0; m < n; m++ )
      Gsub_Free_ChainSubRule( &csr[m], memory );

    FREE( csr );
    return error;
  }


  static void  Gsub_Free_ChainSubRuleSet( TTO_ChainSubRuleSet*  csrs,
				     FT_Memory             memory )
  {
    FT_UShort          n, count;

    TTO_ChainSubRule*  csr;


    if ( csrs->ChainSubRule )
    {
      count = csrs->ChainSubRuleCount;
      csr   = csrs->ChainSubRule;

      for ( n = 0; n < count; n++ )
        Gsub_Free_ChainSubRule( &csr[n], memory );

      FREE( csr );
    }
  }


  /* ChainContextSubstFormat1 */

  static FT_Error  Load_ChainContextSubst1(
                     TTO_ChainContextSubstFormat1*  ccsf1,
                     FT_Stream                      stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort             n = 0, m, count;
    FT_ULong              cur_offset, new_offset, base_offset;

    TTO_ChainSubRuleSet*  csrs;


    base_offset = FILE_Pos() - 2L;

    if ( ACCESS_Frame( 2L ) )
      return error;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_Coverage( &ccsf1->Coverage, stream ) ) != TT_Err_Ok )
      return error;
    (void)FILE_Seek( cur_offset );

    if ( ACCESS_Frame( 2L ) )
      goto Fail2;

    count = ccsf1->ChainSubRuleSetCount = GET_UShort();

    FORGET_Frame();

    ccsf1->ChainSubRuleSet = NULL;

    if ( ALLOC_ARRAY( ccsf1->ChainSubRuleSet, count, TTO_ChainSubRuleSet ) )
      goto Fail2;

    csrs = ccsf1->ChainSubRuleSet;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        goto Fail1;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_ChainSubRuleSet( &csrs[n], stream ) ) != TT_Err_Ok )
        goto Fail1;
      (void)FILE_Seek( cur_offset );
    }

    return TT_Err_Ok;

  Fail1:
    for ( m = 0; m < n; m++ )
      Gsub_Free_ChainSubRuleSet( &csrs[m], memory );

    FREE( csrs );

  Fail2:
    Free_Coverage( &ccsf1->Coverage, memory );
    return error;
  }


  static void  Gsub_Free_ChainContext1( TTO_ChainContextSubstFormat1*  ccsf1,
				   FT_Memory                      memory )
  {
    FT_UShort             n, count;

    TTO_ChainSubRuleSet*  csrs;


    if ( ccsf1->ChainSubRuleSet )
    {
      count = ccsf1->ChainSubRuleSetCount;
      csrs  = ccsf1->ChainSubRuleSet;

      for ( n = 0; n < count; n++ )
        Gsub_Free_ChainSubRuleSet( &csrs[n], memory );

      FREE( csrs );
    }

    Free_Coverage( &ccsf1->Coverage, memory );
  }


  /* ChainSubClassRule */

  static FT_Error  Load_ChainSubClassRule(
                     TTO_ChainContextSubstFormat2*  ccsf2,
                     TTO_ChainSubClassRule*         cscr,
                     FT_Stream                      stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort               n, count;

    FT_UShort*              b;
    FT_UShort*              i;
    FT_UShort*              l;
    TTO_SubstLookupRecord*  slr;
    FT_Bool*                d;


    if ( ACCESS_Frame( 2L ) )
      return error;

    cscr->BacktrackGlyphCount = GET_UShort();

    FORGET_Frame();

    if ( cscr->BacktrackGlyphCount > ccsf2->MaxBacktrackLength )
      ccsf2->MaxBacktrackLength = cscr->BacktrackGlyphCount;

    cscr->Backtrack = NULL;

    count = cscr->BacktrackGlyphCount;

    if ( ALLOC_ARRAY( cscr->Backtrack, count, FT_UShort ) )
      return error;

    b = cscr->Backtrack;
    d = ccsf2->BacktrackClassDef.Defined;

    if ( ACCESS_Frame( count * 2L ) )
      goto Fail4;

    for ( n = 0; n < count; n++ )
    {
      b[n] = GET_UShort();

      /* We check whether the specific class is used at all.  If not,
         class 0 is used instead.                                     */

      if ( !d[b[n]] )
        b[n] = 0;
    }

    FORGET_Frame();

    if ( ACCESS_Frame( 2L ) )
      goto Fail4;

    cscr->InputGlyphCount = GET_UShort();

    FORGET_Frame();

    if ( cscr->InputGlyphCount > ccsf2->MaxInputLength )
      ccsf2->MaxInputLength = cscr->InputGlyphCount;

    cscr->Input = NULL;

    count = cscr->InputGlyphCount - 1; /* only InputGlyphCount - 1 elements */

    if ( ALLOC_ARRAY( cscr->Input, count, FT_UShort ) )
      goto Fail4;

    i = cscr->Input;
    d = ccsf2->InputClassDef.Defined;

    if ( ACCESS_Frame( count * 2L ) )
      goto Fail3;

    for ( n = 0; n < count; n++ )
    {
      i[n] = GET_UShort();

      if ( !d[i[n]] )
        i[n] = 0;
    }

    FORGET_Frame();

    if ( ACCESS_Frame( 2L ) )
      goto Fail3;

    cscr->LookaheadGlyphCount = GET_UShort();

    FORGET_Frame();

    if ( cscr->LookaheadGlyphCount > ccsf2->MaxLookaheadLength )
      ccsf2->MaxLookaheadLength = cscr->LookaheadGlyphCount;

    cscr->Lookahead = NULL;

    count = cscr->LookaheadGlyphCount;

    if ( ALLOC_ARRAY( cscr->Lookahead, count, FT_UShort ) )
      goto Fail3;

    l = cscr->Lookahead;
    d = ccsf2->LookaheadClassDef.Defined;

    if ( ACCESS_Frame( count * 2L ) )
      goto Fail2;

    for ( n = 0; n < count; n++ )
    {
      l[n] = GET_UShort();

      if ( !d[l[n]] )
        l[n] = 0;
    }

    FORGET_Frame();

    if ( ACCESS_Frame( 2L ) )
      goto Fail2;

    cscr->SubstCount = GET_UShort();

    FORGET_Frame();

    cscr->SubstLookupRecord = NULL;

    count = cscr->SubstCount;

    if ( ALLOC_ARRAY( cscr->SubstLookupRecord, count,
                      TTO_SubstLookupRecord ) )
      goto Fail2;

    slr = cscr->SubstLookupRecord;

    if ( ACCESS_Frame( count * 4L ) )
      goto Fail1;

    for ( n = 0; n < count; n++ )
    {
      slr[n].SequenceIndex   = GET_UShort();
      slr[n].LookupListIndex = GET_UShort();
    }

    FORGET_Frame();

    return TT_Err_Ok;

  Fail1:
    FREE( slr );

  Fail2:
    FREE( l );

  Fail3:
    FREE( i );

  Fail4:
    FREE( b );
    return error;
  }


  static void  Gsub_Free_ChainSubClassRule( TTO_ChainSubClassRule*  cscr,
				       FT_Memory               memory )
  {
    FREE( cscr->SubstLookupRecord );
    FREE( cscr->Lookahead );
    FREE( cscr->Input );
    FREE( cscr->Backtrack );
  }


  /* SubClassSet */

  static FT_Error  Load_ChainSubClassSet(
                     TTO_ChainContextSubstFormat2*  ccsf2,
                     TTO_ChainSubClassSet*          cscs,
                     FT_Stream                      stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort               n = 0, m, count;
    FT_ULong                cur_offset, new_offset, base_offset;

    TTO_ChainSubClassRule*  cscr;


    base_offset = FILE_Pos();

    if ( ACCESS_Frame( 2L ) )
      return error;

    count = cscs->ChainSubClassRuleCount = GET_UShort();

    FORGET_Frame();

    cscs->ChainSubClassRule = NULL;

    if ( ALLOC_ARRAY( cscs->ChainSubClassRule, count,
                      TTO_ChainSubClassRule ) )
      return error;

    cscr = cscs->ChainSubClassRule;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        goto Fail;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_ChainSubClassRule( ccsf2, &cscr[n],
                                             stream ) ) != TT_Err_Ok )
        goto Fail;
      (void)FILE_Seek( cur_offset );
    }

    return TT_Err_Ok;

  Fail:
    for ( m = 0; m < n; m++ )
      Gsub_Free_ChainSubClassRule( &cscr[m], memory );

    FREE( cscr );
    return error;
  }


  static void  Gsub_Free_ChainSubClassSet( TTO_ChainSubClassSet*  cscs,
				      FT_Memory              memory )
  {
    FT_UShort               n, count;

    TTO_ChainSubClassRule*  cscr;


    if ( cscs->ChainSubClassRule )
    {
      count = cscs->ChainSubClassRuleCount;
      cscr  = cscs->ChainSubClassRule;

      for ( n = 0; n < count; n++ )
        Gsub_Free_ChainSubClassRule( &cscr[n], memory );

      FREE( cscr );
    }
  }

  static FT_Error Gsub_Load_EmptyOrClassDefinition( TTO_ClassDefinition*  cd,
                                               FT_UShort             limit,
					       FT_ULong              class_offset,
					       FT_ULong              base_offset,
				               FT_Stream             stream )
  {
    FT_Error error;
    FT_ULong               cur_offset;

    cur_offset = FILE_Pos();

    if ( class_offset )
      {
        if ( !FILE_Seek( class_offset + base_offset ) )
          error = Load_ClassDefinition( cd, limit, stream );
      }
    else
       error = Load_EmptyClassDefinition ( cd, stream );

    if (error == TT_Err_Ok)
      (void)FILE_Seek( cur_offset ); /* Changes error as a side-effect */

    return error;
  }


  /* ChainContextSubstFormat2 */

  static FT_Error  Load_ChainContextSubst2(
                     TTO_ChainContextSubstFormat2*  ccsf2,
                     FT_Stream                      stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort              n = 0, m, count;
    FT_ULong               cur_offset, new_offset, base_offset;
    FT_ULong               backtrack_offset, input_offset, lookahead_offset;

    TTO_ChainSubClassSet*  cscs;


    base_offset = FILE_Pos() - 2;

    if ( ACCESS_Frame( 2L ) )
      return error;

    new_offset = GET_UShort() + base_offset;

    FORGET_Frame();

    cur_offset = FILE_Pos();
    if ( FILE_Seek( new_offset ) ||
         ( error = Load_Coverage( &ccsf2->Coverage, stream ) ) != TT_Err_Ok )
      return error;
    (void)FILE_Seek( cur_offset );

    if ( ACCESS_Frame( 8L ) )
      goto Fail5;

    backtrack_offset = GET_UShort();
    input_offset     = GET_UShort();
    lookahead_offset = GET_UShort();

    /* `ChainSubClassSetCount' is the upper limit for input class values,
       thus we read it now to make an additional safety check. No limit
       is known or needed for the other two class definitions          */

    count = ccsf2->ChainSubClassSetCount = GET_UShort();

    FORGET_Frame();

    if ( ( error = Gsub_Load_EmptyOrClassDefinition( &ccsf2->BacktrackClassDef, 65535,
                                                backtrack_offset, base_offset,
					        stream ) ) != TT_Err_Ok )
        goto Fail5;
	       
    if ( ( error = Gsub_Load_EmptyOrClassDefinition( &ccsf2->InputClassDef, count,
                                                input_offset, base_offset,
                                                stream ) ) != TT_Err_Ok )
        goto Fail4;
    if ( ( error = Gsub_Load_EmptyOrClassDefinition( &ccsf2->LookaheadClassDef, 65535,
                                                lookahead_offset, base_offset,
                                                stream ) ) != TT_Err_Ok )
      goto Fail3;

    ccsf2->ChainSubClassSet   = NULL;
    ccsf2->MaxBacktrackLength = 0;
    ccsf2->MaxInputLength     = 0;
    ccsf2->MaxLookaheadLength = 0;

    if ( ALLOC_ARRAY( ccsf2->ChainSubClassSet, count, TTO_ChainSubClassSet ) )
      goto Fail2;

    cscs = ccsf2->ChainSubClassSet;

    for ( n = 0; n < count; n++ )
    {
      if ( ACCESS_Frame( 2L ) )
        goto Fail1;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      if ( new_offset != base_offset )      /* not a NULL offset */
      {
        cur_offset = FILE_Pos();
        if ( FILE_Seek( new_offset ) ||
             ( error = Load_ChainSubClassSet( ccsf2, &cscs[n],
                                              stream ) ) != TT_Err_Ok )
          goto Fail1;
        (void)FILE_Seek( cur_offset );
      }
      else
      {
        /* we create a ChainSubClassSet table with no entries */

        ccsf2->ChainSubClassSet[n].ChainSubClassRuleCount = 0;
        ccsf2->ChainSubClassSet[n].ChainSubClassRule      = NULL;
      }
    }

    return TT_Err_Ok;

  Fail1:
    for ( m = 0; m < n; m++ )
      Gsub_Free_ChainSubClassSet( &cscs[m], memory );

    FREE( cscs );

  Fail2:
    Free_ClassDefinition( &ccsf2->LookaheadClassDef, memory );

  Fail3:
    Free_ClassDefinition( &ccsf2->InputClassDef, memory );

  Fail4:
    Free_ClassDefinition( &ccsf2->BacktrackClassDef, memory );

  Fail5:
    Free_Coverage( &ccsf2->Coverage, memory );
    return error;
  }


  static void  Gsub_Free_ChainContext2( TTO_ChainContextSubstFormat2*  ccsf2,
				   FT_Memory                      memory )
  {
    FT_UShort              n, count;

    TTO_ChainSubClassSet*  cscs;


    if ( ccsf2->ChainSubClassSet )
    {
      count = ccsf2->ChainSubClassSetCount;
      cscs  = ccsf2->ChainSubClassSet;

      for ( n = 0; n < count; n++ )
        Gsub_Free_ChainSubClassSet( &cscs[n], memory );

      FREE( cscs );
    }

    Free_ClassDefinition( &ccsf2->LookaheadClassDef, memory );
    Free_ClassDefinition( &ccsf2->InputClassDef, memory );
    Free_ClassDefinition( &ccsf2->BacktrackClassDef, memory );

    Free_Coverage( &ccsf2->Coverage, memory );
  }


  /* ChainContextSubstFormat3 */

  static FT_Error  Load_ChainContextSubst3(
                     TTO_ChainContextSubstFormat3*  ccsf3,
                     FT_Stream                      stream )
  {
    FT_Error error;
    FT_Memory memory = stream->memory;

    FT_UShort               n, nb = 0, ni =0, nl = 0, m, count;
    FT_UShort               backtrack_count, input_count, lookahead_count;
    FT_ULong                cur_offset, new_offset, base_offset;

    TTO_Coverage*           b;
    TTO_Coverage*           i;
    TTO_Coverage*           l;
    TTO_SubstLookupRecord*  slr;


    base_offset = FILE_Pos() - 2L;

    if ( ACCESS_Frame( 2L ) )
      return error;

    ccsf3->BacktrackGlyphCount = GET_UShort();

    FORGET_Frame();

    ccsf3->BacktrackCoverage = NULL;

    backtrack_count = ccsf3->BacktrackGlyphCount;

    if ( ALLOC_ARRAY( ccsf3->BacktrackCoverage, backtrack_count,
                      TTO_Coverage ) )
      return error;

    b = ccsf3->BacktrackCoverage;

    for ( nb = 0; nb < backtrack_count; nb++ )
    {
      if ( ACCESS_Frame( 2L ) )
        goto Fail4;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_Coverage( &b[nb], stream ) ) != TT_Err_Ok )
        goto Fail4;
      (void)FILE_Seek( cur_offset );
    }

    if ( ACCESS_Frame( 2L ) )
      goto Fail4;

    ccsf3->InputGlyphCount = GET_UShort();

    FORGET_Frame();

    ccsf3->InputCoverage = NULL;

    input_count = ccsf3->InputGlyphCount;

    if ( ALLOC_ARRAY( ccsf3->InputCoverage, input_count, TTO_Coverage ) )
      goto Fail4;

    i = ccsf3->InputCoverage;

    for ( ni = 0; ni < input_count; ni++ )
    {
      if ( ACCESS_Frame( 2L ) )
        goto Fail3;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_Coverage( &i[ni], stream ) ) != TT_Err_Ok )
        goto Fail3;
      (void)FILE_Seek( cur_offset );
    }

    if ( ACCESS_Frame( 2L ) )
      goto Fail3;

    ccsf3->LookaheadGlyphCount = GET_UShort();

    FORGET_Frame();

    ccsf3->LookaheadCoverage = NULL;

    lookahead_count = ccsf3->LookaheadGlyphCount;

    if ( ALLOC_ARRAY( ccsf3->LookaheadCoverage, lookahead_count,
                      TTO_Coverage ) )
      goto Fail3;

    l = ccsf3->LookaheadCoverage;

    for ( nl = 0; nl < lookahead_count; nl++ )
    {
      if ( ACCESS_Frame( 2L ) )
        goto Fail2;

      new_offset = GET_UShort() + base_offset;

      FORGET_Frame();

      cur_offset = FILE_Pos();
      if ( FILE_Seek( new_offset ) ||
           ( error = Load_Coverage( &l[nl], stream ) ) != TT_Err_Ok )
        goto Fail2;
      (void)FILE_Seek( cur_offset );
    }

    if ( ACCESS_Frame( 2L ) )
      goto Fail2;

    ccsf3->SubstCount = GET_UShort();

    FORGET_Frame();

    ccsf3->SubstLookupRecord = NULL;

    count = ccsf3->SubstCount;

    if ( ALLOC_ARRAY( ccsf3->SubstLookupRecord, count,
                      TTO_SubstLookupRecord ) )
      goto Fail2;

    slr = ccsf3->SubstLookupRecord;

    if ( ACCESS_Frame( count * 4L ) )
      goto Fail1;

    for ( n = 0; n < count; n++ )
    {
      slr[n].SequenceIndex   = GET_UShort();
      slr[n].LookupListIndex = GET_UShort();
    }

    FORGET_Frame();

    return TT_Err_Ok;

  Fail1:
    FREE( slr );

  Fail2:
    for ( m = 0; m < nl; m++ )
      Free_Coverage( &l[m], memory );

    FREE( l );

  Fail3:
    for ( m = 0; m < ni; m++ )
      Free_Coverage( &i[m], memory );

    FREE( i );

  Fail4:
    for ( m = 0; m < nb; m++ )
      Free_Coverage( &b[m], memory );

    FREE( b );
    return error;
  }


  static void  Gsub_Free_ChainContext3( TTO_ChainContextSubstFormat3*  ccsf3,
				   FT_Memory                      memory )
  {
    FT_UShort      n, count;

    TTO_Coverage*  c;


    FREE( ccsf3->SubstLookupRecord );

    if ( ccsf3->LookaheadCoverage )
    {
      count = ccsf3->LookaheadGlyphCount;
      c     = ccsf3->LookaheadCoverage;

      for ( n = 0; n < count; n++ )
        Free_Coverage( &c[n], memory );

      FREE( c );
    }

    if ( ccsf3->InputCoverage )
    {
      count = ccsf3->InputGlyphCount;
      c     = ccsf3->InputCoverage;

      for ( n = 0; n < count; n++ )
        Free_Coverage( &c[n], memory );

      FREE( c );
    }

    if ( ccsf3->BacktrackCoverage )
    {
      count = ccsf3->BacktrackGlyphCount;
      c     = ccsf3->BacktrackCoverage;

      for ( n = 0; n < count; n++ )
        Free_Coverage( &c[n], memory );

      FREE( c );
    }
  }


  /* ChainContextSubst */

  FT_Error  Load_ChainContextSubst( TTO_ChainContextSubst*  ccs,
                                    FT_Stream               stream )
  {
    FT_Error error;


    if ( ACCESS_Frame( 2L ) )
      return error;

    ccs->SubstFormat = GET_UShort();

    FORGET_Frame();

    switch ( ccs->SubstFormat )
    {
    case 1:
      return Load_ChainContextSubst1( &ccs->ccsf.ccsf1, stream );

    case 2:
      return Load_ChainContextSubst2( &ccs->ccsf.ccsf2, stream );

    case 3:
      return Load_ChainContextSubst3( &ccs->ccsf.ccsf3, stream );

    default:
      return TTO_Err_Invalid_GSUB_SubTable_Format;
    }

    return TT_Err_Ok;               /* never reached */
  }


  void  Free_ChainContextSubst( TTO_ChainContextSubst*  ccs,
				FT_Memory               memory )
  {
    switch ( ccs->SubstFormat )
    {
    case 1:
      Gsub_Free_ChainContext1( &ccs->ccsf.ccsf1, memory );
      break;

    case 2:
      Gsub_Free_ChainContext2( &ccs->ccsf.ccsf2, memory );
      break;

    case 3:
      Gsub_Free_ChainContext3( &ccs->ccsf.ccsf3, memory );
      break;
    }
  }


  static FT_Error  Lookup_ChainContextSubst1(
                     TTO_GSUBHeader*                gsub,
                     TTO_ChainContextSubstFormat1*  ccsf1,
		     OTL_Buffer                     buffer,
                     FT_UShort                      flags,
                     FT_UShort                      context_length,
                     int                            nesting_level )
  {
    FT_UShort          index, property;
    FT_UShort          i, j, k, num_csr;
    FT_UShort          bgc, igc, lgc;
    FT_Error           error;

    TTO_ChainSubRule*  csr;
    TTO_ChainSubRule   curr_csr;
    TTO_GDEFHeader*    gdef;


    gdef = gsub->gdef;

    if ( CHECK_Property( gdef, IN_CURITEM(), flags, &property ) )
      return error;

    error = Coverage_Index( &ccsf1->Coverage, IN_CURGLYPH(), &index );
    if ( error )
      return error;

    csr     = ccsf1->ChainSubRuleSet[index].ChainSubRule;
    num_csr = ccsf1->ChainSubRuleSet[index].ChainSubRuleCount;

    for ( k = 0; k < num_csr; k++ )
    {
      curr_csr = csr[k];
      bgc      = curr_csr.BacktrackGlyphCount;
      igc      = curr_csr.InputGlyphCount;
      lgc      = curr_csr.LookaheadGlyphCount;

      if ( context_length != 0xFFFF && context_length < igc )
	goto next_chainsubrule;

      /* check whether context is too long; it is a first guess only */

      if ( bgc > buffer->out_pos || buffer->in_pos + igc + lgc > buffer->in_length )
	goto next_chainsubrule;

      if ( bgc )
      {
        /* since we don't know in advance the number of glyphs to inspect,
           we search backwards for matches in the backtrack glyph array    */

        for ( i = 0, j = buffer->out_pos - 1; i < bgc; i++, j-- )
        {
          while ( CHECK_Property( gdef, OUT_ITEM( j ), flags, &property ) )
          {
            if ( error && error != TTO_Err_Not_Covered )
              return error;

	    if ( j + 1 == bgc - i )
	      goto next_chainsubrule;
	    j--;
          }

          /* In OpenType 1.3, it is undefined whether the offsets of
             backtrack glyphs is in logical order or not.  Version 1.4
             will clarify this:

               Logical order -      a  b  c  d  e  f  g  h  i  j
                                                i
               Input offsets -                  0  1
               Backtrack offsets -  3  2  1  0
               Lookahead offsets -                    0  1  2  3           */

          if ( OUT_GLYPH( j ) != curr_csr.Backtrack[i] )
	    goto next_chainsubrule;
        }
      }

      /* Start at 1 because [0] is implied */

      for ( i = 1, j = buffer->in_pos + 1; i < igc; i++, j++ )
      {
        while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
        {
          if ( error && error != TTO_Err_Not_Covered )
            return error;

	  if ( j + igc - i + lgc == (FT_Int)buffer->in_length )
	    goto next_chainsubrule;
	  j++;
        }

        if ( IN_GLYPH( j ) != curr_csr.Input[i - 1] )
	    goto next_chainsubrule;
      }

      /* we are starting to check for lookahead glyphs right after the
         last context glyph                                            */

      for ( i = 0; i < lgc; i++, j++ )
      {
        while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
        {
          if ( error && error != TTO_Err_Not_Covered )
            return error;

	  if ( j + lgc - i == (FT_Int)buffer->in_length )
	    goto next_chainsubrule;
	  j++;
        }

        if ( IN_GLYPH( j ) != curr_csr.Lookahead[i] )
	  goto next_chainsubrule;
      }

      return Do_ContextSubst( gsub, igc,
			      curr_csr.SubstCount,
			      curr_csr.SubstLookupRecord,
			      buffer,
			      nesting_level );

    next_chainsubrule:
      ;
    }

    return TTO_Err_Not_Covered;
  }


  static FT_Error  Lookup_ChainContextSubst2(
                     TTO_GSUBHeader*                gsub,
                     TTO_ChainContextSubstFormat2*  ccsf2,
		     OTL_Buffer                     buffer,
                     FT_UShort                      flags,
                     FT_UShort                      context_length,
                     int                            nesting_level )
  {
    FT_UShort              index, property;
    FT_Memory              memory;
    FT_Error               error;
    FT_UShort              i, j, k;
    FT_UShort              bgc, igc, lgc;
    FT_UShort              known_backtrack_classes,
                           known_input_classes,
                           known_lookahead_classes;

    FT_UShort*             backtrack_classes;
    FT_UShort*             input_classes;
    FT_UShort*             lookahead_classes;

    FT_UShort*             bc;
    FT_UShort*             ic;
    FT_UShort*             lc;

    TTO_ChainSubClassSet*  cscs;
    TTO_ChainSubClassRule  ccsr;
    TTO_GDEFHeader*        gdef;


    gdef = gsub->gdef;
    memory = gsub->memory;

    if ( CHECK_Property( gdef, IN_CURITEM(), flags, &property ) )
      return error;

    /* Note: The coverage table in format 2 doesn't give an index into
             anything.  It just lets us know whether or not we need to
             do any lookup at all.                                     */

    error = Coverage_Index( &ccsf2->Coverage, IN_CURGLYPH(), &index );
    if ( error )
      return error;

    if ( ALLOC_ARRAY( backtrack_classes, ccsf2->MaxBacktrackLength, FT_UShort ) )
      return error;
    known_backtrack_classes = 0;

    if ( ALLOC_ARRAY( input_classes, ccsf2->MaxInputLength, FT_UShort ) )
      goto End3;
    known_input_classes = 1;

    if ( ALLOC_ARRAY( lookahead_classes, ccsf2->MaxLookaheadLength, FT_UShort ) )
      goto End2;
    known_lookahead_classes = 0;

    error = Get_Class( &ccsf2->InputClassDef, IN_CURGLYPH(),
                       &input_classes[0], NULL );
    if ( error && error != TTO_Err_Not_Covered )
      goto End1;

    cscs = &ccsf2->ChainSubClassSet[input_classes[0]];
    if ( !cscs )
    {
      error = TTO_Err_Invalid_GSUB_SubTable;
      goto End1;
    }

    for ( k = 0; k < cscs->ChainSubClassRuleCount; k++ )
    {
      ccsr = cscs->ChainSubClassRule[k];
      bgc  = ccsr.BacktrackGlyphCount;
      igc  = ccsr.InputGlyphCount;
      lgc  = ccsr.LookaheadGlyphCount;

      if ( context_length != 0xFFFF && context_length < igc )
	goto next_chainsubclassrule;

      /* check whether context is too long; it is a first guess only */

      if ( bgc > buffer->out_pos || buffer->in_pos + igc + lgc > buffer->in_length )
	goto next_chainsubclassrule;

      if ( bgc )
      {
        /* Since we don't know in advance the number of glyphs to inspect,
           we search backwards for matches in the backtrack glyph array.
           Note that `known_backtrack_classes' starts at index 0.         */

        bc       = ccsr.Backtrack;

        for ( i = 0, j = buffer->out_pos - 1; i < bgc; i++, j-- )
        {
          while ( CHECK_Property( gdef, OUT_ITEM( j ), flags, &property ) )
          {
            if ( error && error != TTO_Err_Not_Covered )
              goto End1;

	    if ( j + 1 == bgc - i )
	      goto next_chainsubclassrule;
	    j--;
          }

          if ( i >= known_backtrack_classes )
          {
            /* Keeps us from having to do this for each rule */

            error = Get_Class( &ccsf2->BacktrackClassDef, OUT_GLYPH( j ),
                               &backtrack_classes[i], NULL );
            if ( error && error != TTO_Err_Not_Covered )
              goto End1;
            known_backtrack_classes = i;
          }

          if ( bc[i] != backtrack_classes[i] )
	    goto next_chainsubclassrule;
        }
      }

      ic       = ccsr.Input;

      /* Start at 1 because [0] is implied */

      for ( i = 1, j = buffer->in_pos + 1; i < igc; i++, j++ )
      {
        while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
        {
          if ( error && error != TTO_Err_Not_Covered )
            goto End1;

	  if ( j + igc - i + lgc == (FT_Int)buffer->in_length )
	    goto next_chainsubclassrule;
	  j++;
        }

        if ( i >= known_input_classes )
        {
          error = Get_Class( &ccsf2->InputClassDef, IN_GLYPH( j ),
                             &input_classes[i], NULL );
          if ( error && error != TTO_Err_Not_Covered )
            goto End1;
          known_input_classes = i;
        }

        if ( ic[i - 1] != input_classes[i] )
	  goto next_chainsubclassrule;
      }

      /* we are starting to check for lookahead glyphs right after the
         last context glyph                                            */

      lc       = ccsr.Lookahead;

      for ( i = 0; i < lgc; i++, j++ )
      {
        while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
        {
          if ( error && error != TTO_Err_Not_Covered )
            goto End1;

	  if ( j + lgc - i == (FT_Int)buffer->in_length )
	    goto next_chainsubclassrule;
	  j++;
        }

        if ( i >= known_lookahead_classes )
        {
          error = Get_Class( &ccsf2->LookaheadClassDef, IN_GLYPH( j ),
                             &lookahead_classes[i], NULL );
          if ( error && error != TTO_Err_Not_Covered )
            goto End1;
          known_lookahead_classes = i;
        }

        if ( lc[i] != lookahead_classes[i] )
	  goto next_chainsubclassrule;
      }

      error = Do_ContextSubst( gsub, igc,
			       ccsr.SubstCount,
			       ccsr.SubstLookupRecord,
			       buffer,
			       nesting_level );
      goto End1;

    next_chainsubclassrule:
      ;
    }

    error = TTO_Err_Not_Covered;

  End1:
    FREE( lookahead_classes );

  End2:
    FREE( input_classes );

  End3:
    FREE( backtrack_classes );
    return error;
  }


  static FT_Error  Lookup_ChainContextSubst3(
                     TTO_GSUBHeader*                gsub,
                     TTO_ChainContextSubstFormat3*  ccsf3,
		     OTL_Buffer                     buffer,
                     FT_UShort                      flags,
                     FT_UShort                      context_length,
                     int                            nesting_level )
  {
    FT_UShort        index, i, j, property;
    FT_UShort        bgc, igc, lgc;
    FT_Error         error;

    TTO_Coverage*    bc;
    TTO_Coverage*    ic;
    TTO_Coverage*    lc;
    TTO_GDEFHeader*  gdef;


    gdef = gsub->gdef;

    if ( CHECK_Property( gdef, IN_CURITEM(), flags, &property ) )
      return error;

    bgc = ccsf3->BacktrackGlyphCount;
    igc = ccsf3->InputGlyphCount;
    lgc = ccsf3->LookaheadGlyphCount;

    if ( context_length != 0xFFFF && context_length < igc )
      return TTO_Err_Not_Covered;

    /* check whether context is too long; it is a first guess only */

    if ( bgc > buffer->out_pos || buffer->in_pos + igc + lgc > buffer->in_length )
      return TTO_Err_Not_Covered;

    if ( bgc )
    {
      /* Since we don't know in advance the number of glyphs to inspect,
         we search backwards for matches in the backtrack glyph array    */

      bc       = ccsf3->BacktrackCoverage;

      for ( i = 0, j = buffer->out_pos - 1; i < bgc; i++, j-- )
      {
        while ( CHECK_Property( gdef, OUT_ITEM( j ), flags, &property ) )
        {
          if ( error && error != TTO_Err_Not_Covered )
            return error;

	  if ( j + 1 == bgc - i )
	    return TTO_Err_Not_Covered;
	  j--;
        }

        error = Coverage_Index( &bc[i], OUT_GLYPH( j ), &index );
        if ( error )
          return error;
      }
    }

    ic       = ccsf3->InputCoverage;

    for ( i = 0, j = buffer->in_pos; i < igc; i++, j++ )
    {
      /* We already called CHECK_Property for IN_GLYPH( buffer->in_pos ) */
      while ( j > buffer->in_pos && CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
      {
        if ( error && error != TTO_Err_Not_Covered )
          return error;
	
	if ( j + igc - i + lgc == (FT_Int)buffer->in_length )
          return TTO_Err_Not_Covered;
	j++;
      }

      error = Coverage_Index( &ic[i], IN_GLYPH( j ), &index );
      if ( error )
        return error;
    }

    /* we are starting for lookahead glyphs right after the last context
       glyph                                                             */

    lc       = ccsf3->LookaheadCoverage;

    for ( i = 0; i < lgc; i++, j++ )
    {
      while ( CHECK_Property( gdef, IN_ITEM( j ), flags, &property ) )
      {
        if ( error && error != TTO_Err_Not_Covered )
          return error;

	if ( j + lgc - i == (FT_Int)buffer->in_length )
	  return TTO_Err_Not_Covered;
	j++;
      }

      error = Coverage_Index( &lc[i], IN_GLYPH( j ), &index );
      if ( error )
        return error;
    }

    return Do_ContextSubst( gsub, igc,
                            ccsf3->SubstCount,
                            ccsf3->SubstLookupRecord,
			    buffer,
                            nesting_level );
  }


  static FT_Error  Lookup_ChainContextSubst(
                     TTO_GSUBHeader*         gsub,
                     TTO_ChainContextSubst*  ccs,
		     OTL_Buffer              buffer,
                     FT_UShort               flags,
                     FT_UShort               context_length,
                     int                     nesting_level )
  {
    switch ( ccs->SubstFormat )
    {
    case 1:
      return Lookup_ChainContextSubst1( gsub, &ccs->ccsf.ccsf1, buffer,
                                        flags, context_length,
                                        nesting_level );

    case 2:
      return Lookup_ChainContextSubst2( gsub, &ccs->ccsf.ccsf2, buffer,
                                        flags, context_length,
                                        nesting_level );

    case 3:
      return Lookup_ChainContextSubst3( gsub, &ccs->ccsf.ccsf3, buffer,
                                        flags, context_length,
                                        nesting_level );

    default:
      return TTO_Err_Invalid_GSUB_SubTable_Format;
    }

    return TT_Err_Ok;               /* never reached */
  }



  /***********
   * GSUB API
   ***********/


  EXPORT_FUNC
  FT_Error  TT_GSUB_Select_Script( TTO_GSUBHeader*  gsub,
                                   FT_ULong         script_tag,
                                   FT_UShort*       script_index )
  {
    FT_UShort          n;

    TTO_ScriptList*    sl;
    TTO_ScriptRecord*  sr;


    if ( !gsub || !script_index )
      return TT_Err_Invalid_Argument;

    sl = &gsub->ScriptList;
    sr = sl->ScriptRecord;

    for ( n = 0; n < sl->ScriptCount; n++ )
      if ( script_tag == sr[n].ScriptTag )
      {
        *script_index = n;

        return TT_Err_Ok;
      }

    return TTO_Err_Not_Covered;
  }


  EXPORT_FUNC
  FT_Error  TT_GSUB_Select_Language( TTO_GSUBHeader*  gsub,
                                     FT_ULong         language_tag,
                                     FT_UShort        script_index,
                                     FT_UShort*       language_index,
                                     FT_UShort*       req_feature_index )
  {
    FT_UShort           n;

    TTO_ScriptList*     sl;
    TTO_ScriptRecord*   sr;
    TTO_Script*         s;
    TTO_LangSysRecord*  lsr;


    if ( !gsub || !language_index || !req_feature_index )
      return TT_Err_Invalid_Argument;

    sl = &gsub->ScriptList;
    sr = sl->ScriptRecord;

    if ( script_index >= sl->ScriptCount )
      return TT_Err_Invalid_Argument;

    s   = &sr[script_index].Script;
    lsr = s->LangSysRecord;

    for ( n = 0; n < s->LangSysCount; n++ )
      if ( language_tag == lsr[n].LangSysTag )
      {
        *language_index = n;
        *req_feature_index = lsr[n].LangSys.ReqFeatureIndex;

        return TT_Err_Ok;
      }

    return TTO_Err_Not_Covered;
  }


  /* selecting 0xFFFF for language_index asks for the values of the
     default language (DefaultLangSys)                              */

  EXPORT_FUNC
  FT_Error  TT_GSUB_Select_Feature( TTO_GSUBHeader*  gsub,
                                    FT_ULong         feature_tag,
                                    FT_UShort        script_index,
                                    FT_UShort        language_index,
                                    FT_UShort*       feature_index )
  {
    FT_UShort           n;

    TTO_ScriptList*     sl;
    TTO_ScriptRecord*   sr;
    TTO_Script*         s;
    TTO_LangSysRecord*  lsr;
    TTO_LangSys*        ls;
    FT_UShort*          fi;

    TTO_FeatureList*    fl;
    TTO_FeatureRecord*  fr;


    if ( !gsub || !feature_index )
      return TT_Err_Invalid_Argument;

    sl = &gsub->ScriptList;
    sr = sl->ScriptRecord;

    fl = &gsub->FeatureList;
    fr = fl->FeatureRecord;

    if ( script_index >= sl->ScriptCount )
      return TT_Err_Invalid_Argument;

    s   = &sr[script_index].Script;
    lsr = s->LangSysRecord;

    if ( language_index == 0xFFFF )
      ls = &s->DefaultLangSys;
    else
    {
      if ( language_index >= s->LangSysCount )
        return TT_Err_Invalid_Argument;

      ls = &lsr[language_index].LangSys;
    }

    fi = ls->FeatureIndex;

    for ( n = 0; n < ls->FeatureCount; n++ )
    {
      if ( fi[n] >= fl->FeatureCount )
        return TTO_Err_Invalid_GSUB_SubTable_Format;

      if ( feature_tag == fr[fi[n]].FeatureTag )
      {
        *feature_index = fi[n];

        return TT_Err_Ok;
      }
    }

    return TTO_Err_Not_Covered;
  }


  /* The next three functions return a null-terminated list */

  EXPORT_FUNC
  FT_Error  TT_GSUB_Query_Scripts( TTO_GSUBHeader*  gsub,
                                   FT_ULong**       script_tag_list )
  {
    FT_UShort          n;
    FT_Error           error;
    FT_Memory          memory;
    FT_ULong*          stl;

    TTO_ScriptList*    sl;
    TTO_ScriptRecord*  sr;


    if ( !gsub || !script_tag_list )
      return TT_Err_Invalid_Argument;

    memory = gsub->memory;

    sl = &gsub->ScriptList;
    sr = sl->ScriptRecord;

    if ( ALLOC_ARRAY( stl, sl->ScriptCount + 1, FT_ULong ) )
      return error;

    for ( n = 0; n < sl->ScriptCount; n++ )
      stl[n] = sr[n].ScriptTag;
    stl[n] = 0;

    *script_tag_list = stl;

    return TT_Err_Ok;
  }


  EXPORT_FUNC
  FT_Error  TT_GSUB_Query_Languages( TTO_GSUBHeader*  gsub,
                                     FT_UShort        script_index,
                                     FT_ULong**       language_tag_list )
  {
    FT_UShort           n;
    FT_Error            error;
    FT_Memory           memory;
    FT_ULong*           ltl;

    TTO_ScriptList*     sl;
    TTO_ScriptRecord*   sr;
    TTO_Script*         s;
    TTO_LangSysRecord*  lsr;


    if ( !gsub || !language_tag_list )
      return TT_Err_Invalid_Argument;

    memory = gsub->memory;
    
    sl = &gsub->ScriptList;
    sr = sl->ScriptRecord;

    if ( script_index >= sl->ScriptCount )
      return TT_Err_Invalid_Argument;

    s   = &sr[script_index].Script;
    lsr = s->LangSysRecord;

    if ( ALLOC_ARRAY( ltl, s->LangSysCount + 1, FT_ULong ) )
      return error;

    for ( n = 0; n < s->LangSysCount; n++ )
      ltl[n] = lsr[n].LangSysTag;
    ltl[n] = 0;

    *language_tag_list = ltl;

    return TT_Err_Ok;
  }


  /* selecting 0xFFFF for language_index asks for the values of the
     default language (DefaultLangSys)                              */

  EXPORT_FUNC
  FT_Error  TT_GSUB_Query_Features( TTO_GSUBHeader*  gsub,
                                    FT_UShort        script_index,
                                    FT_UShort        language_index,
                                    FT_ULong**       feature_tag_list )
  {
    FT_UShort           n;
    FT_Error            error;
    FT_Memory           memory;
    FT_ULong*           ftl;

    TTO_ScriptList*     sl;
    TTO_ScriptRecord*   sr;
    TTO_Script*         s;
    TTO_LangSysRecord*  lsr;
    TTO_LangSys*        ls;
    FT_UShort*          fi;

    TTO_FeatureList*    fl;
    TTO_FeatureRecord*  fr;


    if ( !gsub || !feature_tag_list )
      return TT_Err_Invalid_Argument;

    memory = gsub->memory;
    
    sl = &gsub->ScriptList;
    sr = sl->ScriptRecord;

    fl = &gsub->FeatureList;
    fr = fl->FeatureRecord;

    if ( script_index >= sl->ScriptCount )
      return TT_Err_Invalid_Argument;

    s   = &sr[script_index].Script;
    lsr = s->LangSysRecord;

    if ( language_index == 0xFFFF )
      ls = &s->DefaultLangSys;
    else
    {
      if ( language_index >= s->LangSysCount )
        return TT_Err_Invalid_Argument;

      ls = &lsr[language_index].LangSys;
    }

    fi = ls->FeatureIndex;

    if ( ALLOC_ARRAY( ftl, ls->FeatureCount + 1, FT_ULong ) )
      return error;

    for ( n = 0; n < ls->FeatureCount; n++ )
    {
      if ( fi[n] >= fl->FeatureCount )
      {
        FREE( ftl );
        return TTO_Err_Invalid_GSUB_SubTable_Format;
      }
      ftl[n] = fr[fi[n]].FeatureTag;
    }
    ftl[n] = 0;

    *feature_tag_list = ftl;

    return TT_Err_Ok;
  }


  /* Do an individual subtable lookup.  Returns TT_Err_Ok if substitution
     has been done, or TTO_Err_Not_Covered if not.                        */

  static FT_Error  GSub_Do_Glyph_Lookup( TTO_GSUBHeader*   gsub,
                                         FT_UShort         lookup_index,
                                         OTL_Buffer        buffer,
                                         FT_UShort         context_length,
                                         int               nesting_level )
  {
    FT_Error     error = TTO_Err_Not_Covered;
    FT_UShort    i, flags, lookup_count;
    TTO_Lookup*  lo;


    nesting_level++;

    if ( nesting_level > TTO_MAX_NESTING_LEVEL )
      return TTO_Err_Too_Many_Nested_Contexts;

    lookup_count = gsub->LookupList.LookupCount;
    if (lookup_index >= lookup_count)
      return error;

    lo    = &gsub->LookupList.Lookup[lookup_index];
    flags = lo->LookupFlag;

    for ( i = 0; i < lo->SubTableCount; i++ )
    {
      switch ( lo->LookupType )
      {
      case GSUB_LOOKUP_SINGLE:
        error = Lookup_SingleSubst( &lo->SubTable[i].st.gsub.single,
                                    buffer,
                                    flags, context_length, gsub->gdef );
        break;

      case GSUB_LOOKUP_MULTIPLE:
        error = Lookup_MultipleSubst( &lo->SubTable[i].st.gsub.multiple,
                                      buffer,
                                      flags, context_length, gsub->gdef );
        break;

      case GSUB_LOOKUP_ALTERNATE:
        error = Lookup_AlternateSubst( gsub,
                                       &lo->SubTable[i].st.gsub.alternate,
                                       buffer,
                                       flags, context_length, gsub->gdef );
        break;

      case GSUB_LOOKUP_LIGATURE:
        error = Lookup_LigatureSubst( &lo->SubTable[i].st.gsub.ligature,
				      buffer,
                                      flags, context_length, gsub->gdef );
        break;

      case GSUB_LOOKUP_CONTEXT:
        error = Lookup_ContextSubst( gsub, &lo->SubTable[i].st.gsub.context,
                                     buffer,
                                     flags, context_length, nesting_level );
        break;

      case GSUB_LOOKUP_CHAIN:
        error = Lookup_ChainContextSubst( gsub,
                                          &lo->SubTable[i].st.gsub.chain,
					  buffer,
                                          flags, context_length,
                                          nesting_level );
        break;
      }

      /* Check whether we have a successful substitution or an error other
         than TTO_Err_Not_Covered                                          */

      if ( error != TTO_Err_Not_Covered )
        return error;
    }

    return TTO_Err_Not_Covered;
  }

  /* apply one lookup to the input string object */

  static FT_Error  GSub_Do_String_Lookup( TTO_GSUBHeader*   gsub,
                                          FT_UShort         lookup_index,
                                          OTL_Buffer        buffer )
  {
    FT_Error  error, retError = TTO_Err_Not_Covered;

    FT_UInt*  properties = gsub->LookupList.Properties;

    int      nesting_level = 0;


    while ( buffer->in_pos < buffer->in_length )
    {
      if ( ~IN_PROPERTIES( buffer->in_pos ) & properties[lookup_index] )
      {
        /* 0xFFFF indicates that we don't have a context length yet */
        error = GSub_Do_Glyph_Lookup( gsub, lookup_index, buffer,
                                      0xFFFF, nesting_level );
        if ( error )
	{
	  if ( error != TTO_Err_Not_Covered )
	    return error;
	}
	else
	  retError = error;
      }
      else
        error = TTO_Err_Not_Covered;

      if ( error == TTO_Err_Not_Covered )
        if ( otl_buffer_copy_output_glyph ( buffer ) )
          return error;
    }

    return retError;
  }


  EXPORT_FUNC
  FT_Error  TT_GSUB_Add_Feature( TTO_GSUBHeader*  gsub,
                                 FT_UShort        feature_index,
                                 FT_UInt          property )
  {
    FT_UShort    i;

    TTO_Feature  feature;
    FT_UInt*     properties;
    FT_UShort*   index;
    FT_UShort    lookup_count;

    /* Each feature can only be added once */
    
    if ( !gsub ||
         feature_index >= gsub->FeatureList.FeatureCount ||
	 gsub->FeatureList.ApplyCount == gsub->FeatureList.FeatureCount )
      return TT_Err_Invalid_Argument;

    gsub->FeatureList.ApplyOrder[gsub->FeatureList.ApplyCount++] = feature_index;

    properties = gsub->LookupList.Properties;

    feature = gsub->FeatureList.FeatureRecord[feature_index].Feature;
    index   = feature.LookupListIndex;
    lookup_count = gsub->LookupList.LookupCount;

    for ( i = 0; i < feature.LookupListCount; i++ )
    {
      FT_UShort lookup_index = index[i];
      if (lookup_index < lookup_count)
	properties[lookup_index] |= property;
    }

    return TT_Err_Ok;
  }


  EXPORT_FUNC
  FT_Error  TT_GSUB_Clear_Features( TTO_GSUBHeader*  gsub )
  {
    FT_UShort i;

    FT_UInt*  properties;


    if ( !gsub )
      return TT_Err_Invalid_Argument;

    gsub->FeatureList.ApplyCount = 0;

    properties = gsub->LookupList.Properties;

    for ( i = 0; i < gsub->LookupList.LookupCount; i++ )
      properties[i] = 0;

    return TT_Err_Ok;
  }


  EXPORT_FUNC
  FT_Error  TT_GSUB_Register_Alternate_Function( TTO_GSUBHeader*  gsub,
                                                 TTO_AltFunction  altfunc,
                                                 void*            data )
  {
    if ( !gsub )
      return TT_Err_Invalid_Argument;

    gsub->altfunc = altfunc;
    gsub->data    = data;

    return TT_Err_Ok;
  }


  EXPORT_FUNC
  FT_Error  TT_GSUB_Apply_String( TTO_GSUBHeader*   gsub,
				  OTL_Buffer        buffer )
  {
    FT_Error          error, retError = TTO_Err_Not_Covered;
    FT_UShort         i, j, feature_index, lookup_count;
    TTO_Feature       feature;

    if ( !gsub ||
         !buffer || buffer->in_length == 0 || buffer->in_pos >= buffer->in_length )
      return TT_Err_Invalid_Argument;

    lookup_count = gsub->LookupList.LookupCount;

    for ( i = 0; i < gsub->FeatureList.ApplyCount; i++)
    {
      feature_index = gsub->FeatureList.ApplyOrder[i];
      feature = gsub->FeatureList.FeatureRecord[feature_index].Feature;

      for ( j = 0; j < feature.LookupListCount; j++ )
      {
	FT_UShort lookup_index = feature.LookupListIndex[j];

	/* Skip nonexistant lookups */
        if (lookup_index >= lookup_count)
	 continue;

        error = GSub_Do_String_Lookup( gsub, lookup_index, buffer );
	if ( error )
	{
	  if ( error != TTO_Err_Not_Covered )
	    goto End;
	}
	else
	  retError = error;
        
	error = otl_buffer_swap( buffer );
	if ( error )
	  goto End;
      } 
    }
    
    error = retError;

  End:
    return error;
  }


/* END */
