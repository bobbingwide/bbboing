<?php // (C) Copyright Bobbing Wide 2010-2016

/**
 * Implement [bbboing] shortcode 
 *
 * @param array $atts shortcode parameters
 * @param string $content - not implemented
 * @param string $tag 
 * @return string - the result of the shortcode expansion  
 */
function bbboing_sc( $atts=null, $content=null, $tag=null ) {
  $form = bw_array_get( $atts, "form", 'N' );
  $both = bw_array_get( $atts, "both", 'N' );
  $text = bw_array_get_dcb( $atts, "text", "", "seed" );
  $form = bw_validate_torf( $form );
  $both = bw_validate_torf( $both );
  if ( $form ) {
    $result = _bbboing_form( $text, $atts );
  } else {
    $result = _bbboing_static( $text, $both );
  }   
  return( $result );
}

/**
 * Display the original text and the bbboing'ed text
 * 
 * @param string $text - the text to be "bboinged"
 * @param bool $both - whether or not to display the original text and the result
 * @return string - the result
 */
function _bbboing_static( $text=null, $both=false ) {
  if ( $both ) {
    h4( __("Original text", "bbboing" ) );
    p( $text );
    h4( __( "bbboing'ed", "bbboing" ) );
  }
  p( bbboing( $text ));
  return( bw_ret());
}

/** 
 * Return example text for bbboing
 * 
 * Should we really make this translatable? 
 * 
 * @return string - the original intended text
 * 
 */
function seed() {
  $bnobbig = "According to a researcher at Cambridge University, it doesn't matter in which order the letters in a word are, ";
  $bnobbig.= "the only important thing is that the first and last letter be at the right place. ";
  $bnobbig.= "The rest can be a total mess and you can still read it without problem. "; 
  $bnobbig.= "This is because the human mind does not read every letter by itself but the word as a whole. ";
  return( $bnobbig );
}

/**
 * Return the original version where the bbboing'ing of "researcher" was incorrectly shown as "rscheearch"
 * 
 * You can work out all the differences for yourself
 * 
 * @return string - the "hoax" version
 */
function hoax_version() {
  $bnobbig = "According to a researcher at Cambridge University, it doesn't matter in which order the letters in a word are,";
  
  $bnobbig = "Aoccdrnig to a rscheearch at Cmabrigde Uinervtisy, it deosn't mttaer in waht oredr the ltteers in a wrod are, ";
  $bnobbig.= "the olny iprmoetnt tihng is taht the frist and lsat ltteer be at the rghit pclae. ";
  $bnobbig.= "The rset can be a toatl mses and you can sitll raed it wouthit porbelm. ";
  $bnobbig.= "Tihs is bcuseae the huamn mnid deos not raed ervey lteter by istlef but the wrod as a wlohe. ";
  return( $bnobbig );
}   

/** 
 * Return the text to be bbboing'ed with some sanitization
 * 
 * Nonce verification is also performed. 
 * 
 * @param string $text - default text
 * @return string - sanitized text
 */
function _bbboing_get( $text=null ) {
  $verified = bw_verify_nonce( "_bbboing_form", "_bbboing_nonce" );
  if ( $verified ) {
    $new_text = bw_array_get( $_REQUEST, "_bbboing_text", $text );
    $new_text = str_replace( "\n", "&#8288;", $new_text );
    $new_text = strip_tags( $new_text );
    $new_text = stripslashes( $new_text ); 
  } else {
    $new_text = $text;
  }    
  return( $new_text );
}     
 
/**
 * Display the form to accept text to be bbboing'ed 
 * 
 * Note: removed class="inline" from the form. **?** is this OK?
 *
 * @param string $text 
 * @param array $atts 
 * @return string generated HTML 
 */
function _bbboing_form( $text=null, $atts=null ) {
	bw_context( "textdomain", "bbboing" );
  oik_require( "bobbforms.inc" );
  $text = _bbboing_get( $text );  
  $cols = bw_array_get( $atts, "cols", 80 );
  bw_form();
  p( "Type the text you want to obfuscate then click on 'bbboing it'" );
  bw_textarea( "_bbboing_text", $cols, null, $text );
  br();
  e( wp_nonce_field( "_bbboing_form", "_bbboing_nonce", false, false ) );
  e( isubmit( "_bbboing_ok", __( "bbboing it", "bbboing" ) ) );
  p("This is the result of <i>bbboing</i>ing the text." ); 
  bw_textarea( "_bbboinged", $cols, null ,   bbboing( $text ) );
  p( "If you're not happy with the result just click on 'bbboing it'!" );
  etag( "form" );
	
	bw_context( "textdomain", false );
  return( bw_ret());
}

/** 
 * bbboing some text
 * 
 * Note: In its first version this routine would transform "http://www.bobbingwide.com" to something pretty nasty
 * similarly it could ruin any HTML tags or anything with %1$s
 * To overcome this we're going to improve the bboing() function... replacing it with the new function bboing2()
 * 
 * @param string - the text to be bbboing'ed
 * @return string - the bbboing'ed text
 */
function bbboing( $text ) {
  $bbtext = '';
  $words = explode( ' ', $text );
  foreach ( $words as $word ) {
    $bbtext.= bboing2($word );   
    $bbtext.= " ";
  }
  $bbtext = trim( $bbtext );
  return( $bbtext );
}

/**
 * bbboing a particular (potentially complex) word
 *
 * If the word itself did not start with a "<" then the original logic attempted to ignore any final punctuation character
 * So "word." would become "wrod."
 * 
 * But we need to be smarter than that. 
 * 
 * e.g. 
 * http://www.bobbingwide.com could become
 * http://www.bbbidiwonge.com      
 *
 * @param string $word - a particular word
 * @return string - the bbboing'ed word
 */
function bboing( $word ) {
  if ( ctype_alpha( $word ) ) {
    // echo "c";
    $word = boing( $word ); 
  } else {
    $count = strlen( $word );
    $l = substr( $word, 0, 1 );
    if ( $l <> "<" ) {
      $r = substr( $word, $count-1 ); 
      $pos = strpos( ".?:-", $r );
      if ($pos === FALSE ) {
         $word = boing( $word ); 
      } else { 
        $word = boing( substr( $word, 0, $count-1 )) . $r;
      }   
    }
  } 
  return( $word );
}

/**
 * Shuffle the embedded letters in a word of 4 or more characters
 * 
 * The first and last letters remain the same, all the others are randomised.
 * e.g.
 * bobbing could become bbboing or bibbong or bnibbog
 *
 * @param string $word
 * @return string $wrod
 */ 
function boing( $word ) {
  // echo $word . "\n";
  $count = strlen( $word );
  switch ( $count )
  {
    case 0:
    case 1:
    case 2:
    case 3:
      // Can't really do anything with this length word
      // Convert all vowels to uppercase
      // Convert a to decimal 132 hex 84 - which is  „  - a umlaut 
      // Convert e to decimal 130 hex 82 - which is  ‚  - e acute
      // Convert i to decimal 140 hex 8C - which is  Œ  - i caret
      // Convert o to decimal 149 hex 95 - which is  •  - o grave
      // Convert u to decimal 129 hex 81 - which is    - u umlaut
      // Don't do this in the bbboing plugin, just in the bb_BB language generator.
      // $wrod = str_replace( array( "a", "e", "i", "o", "u" ), array( "A", "E", "I", "O", "U" ), $word );
      // $wrod = str_replace( array( "a", "e", "i", "o", "u" ), array( "„", "‚", "Œ", "•", "" ), $word );
      $wrod = $word;
      break;

    default:
      $l = substr( $word, 0, 1 );
      $r = substr( $word, $count-1 ); 
      $mid = substr( $word, 1, $count-2 ); 
    
      $dim = str_shuffle( $mid );
      if ( $dim == $mid ) {
        $dim = str_shuffle( $mid );
      }  
      $wrod = $l . $dim . $r;
      break;
  }    
  return( $wrod );
}

/**
 * Perform boing() against a part of the word replacing the characters in the chars array
 *
 * @param string $chars 
 * @param string $subword
 * @param integer $first
 */
function boingsubword( &$chars, $subword, $first ) {
  $wrod = boing( $subword );
  $crahs = str_split( $wrod );
  foreach ( $crahs as $i => $char ) {
    $chars[$i+$first] = $char;
  }  
}

/**
 * Return true if we think we should process the word, false otherwise
 * 
 * Note: We expect the words to be split on blank characters
 * 
 * Strings we should not auto translate contain:
 *
 * Value | Example usage
 * ----- | ---------------------------------------------------
 * :// | URLs  http://,  https://, ftp://, ftps://, mailto://
 * @ | email addresses
 * % | symbolic substitution variables e.g. %1$s 
 * = | keyword="value" ... but this could have problems if within HTML tags...
 * & | symbolic html such as &lasquo; so we cater for < and > separately
 *
 * @param string $word
 * @return bool true if we should process the word 
 */
function bboing_process_word( $word ) {
  $process = true;
  $ignores = array( "://", "@", "%", "=", "&", "[", "]" );
  foreach ( $ignores as $ignore ) {
    if ( strpos( $word, $ignore ) !== false ) {
      $process = false;
      break;
    }  
  }
  $reportable = array( "<", ">" );
  foreach ( $reportable as $report ) {
    if ( strpos( $word, $report ) !== false ) {
      $process = false;
      // echo "#. Possible embedded HTML in string. Consider changing code\n" ;
      break;
    }  
  }
  return( $process );
}  

/** 
 * Only bbboing characters inside a complex string
 *
 * bboing only the character parts of a continuous string of characters in a word
 * with some sanity checking on what we're messing with
 * 
 * Punctuation characters used to delimit the bbboinging are:
 * -.:;?' - 
 * well anything else really except characters
 * we don't even accept digits
 * 
 * @param string $word
 * @return string $wrod
 */
function bboing2( $word ) {
  if ( bboing_process_word( $word ) ) {
    $chars = str_split( $word );
    $count = 0;
    $first = null;
    $subword = null;
    foreach ( $chars as $index => $char ) {
      if ( ctype_alpha( $char ) ) {
        $count++;
        if ( $first === null ) {
          $first = $index;
        }
        $subword .= $char;
      } else {
        if ( $count > 0 ) {
          boingsubword( $chars, $subword, $first );
        }         
        $count=0;
        $first=null; 
        $subword = null;
      }  
    } 
    if ( $count > 0 ) {
      boingsubword( $chars, $subword, $first );
    }
    $wrod = implode( $chars );
  } else {
    $wrod = $word; 
  }  
  return( $wrod );
}

/** 
 * Help for bbboing
 */
function bbboing__help( ) {
 return( __( "Obfuscate some text but leave it readable, apparently", "bbboing") );
} 

/**
 * Syntax for bbboing
 * 
 */
function bbboing__syntax( ) {
  $syntax = array( "text" => bw_skv( seed(), "textarea", "some text to obfuscate" )
                 , "form" => bw_skv( "N", "Y", "display form to allow changes" )
                 , "both" => bw_skv( "N", "Y", "display both original and output text" )
                 );
  return( $syntax );
}

/**
 * Example for the bbboing shortcode
 *  
 * [bbboing text="and this is where it shows the before and after" both='Y']
 * 
 */
function bbboing__example( ) {
  oik_require( "bbboing.inc", "bbboing" );
  e ( _bbboing_static( seed() ) );
  h4( "The manual version" );
  p( "This is the original obfuscated text. It has been manually adjusted to make it readable" );
  p( hoax_version() );
}             

/**
 * Verify the nonce field
 * @param string $action - the action passed on the call to wp_nonce_field()
 * @param string $name - the name passed on the call to wp_nonce_field() 
 * @return bool - 1 or 2 if verified, false if not
 * 
 */
if ( !function_exists( "bw_verify_nonce" ) ) {
function bw_verify_nonce( $action, $name ) {
  $nonce_field = bw_array_get( $_REQUEST, $name, null );
  $verified = wp_verify_nonce( $nonce_field, $action );
  bw_trace2( $verified, "wp_verify_nonce?" );
  return( $verified );
}  
}
 

