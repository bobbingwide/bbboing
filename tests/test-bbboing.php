<?php // (C) Copyright Bobbing Wide 2016
/**
 * @group bbboing
 */
class Tests_bbboing extends WP_UnitTestCase {

	/**
	 * setUp is run for every test
	 */
	function setUp() {
		// parent::setUp();
		//do_action( "oik_add_shortcodes" );
		//echo "setUp run";
	}

	/**
	 * tearDown is run for every test
	 */
	function tearDown() {
		// parent::tearDown();
		//echo "tearDown run";
	}

	/**
	 * Demonstrate that bbboing is active
	 */
	function test_bbboing_loaded_action_performed() {
		$performed = did_action( "bbboing_loaded" );
		$this->assertEquals( 1, $performed );
	}

	/**
	 * Ensure that oik shortcodes can be run
	 */
	function test_oik_add_shortcodes() {
		$performed = did_action( "oik_add_shortcodes" );
		if ( !$performed ) {
			do_action( "oik_add_shortcodes" );
		}
		$performed = did_action( "oik_add_shortcodes" );
		$this->assertEquals( 1, $performed );
	}

	/**
	 * Test the bbboing shortcode form=n both=y
	 * 
	 *
	 * bbboing is a lazy smart shortcode so the functions won't be loaded
	 * until you execute it. 
	 * 
	 */
	function test_bbboing_form_n_both_y() {
		$result = do_shortcode( "[bbboing form=n both=y text=Anything] ");
		$this->assertStringStartsWith( "<h4>", $result );
		bw_trace2( $result );
	}

	/**
	 * Test bbboing form=y
	 */
	function test_bbboing_form_y() {
		$result = do_shortcode( "[bbboing form=y]" );
		$this->assertStringStartsWith( '<form method="post">', $result );
		bw_trace2( $result );
	}

	/**
	 * Test [bbboing] on its own.
	 * 
	 * We should also test the ends with and perhaps the length.
	 *  
	 */
	function test_bbboing() {
		$result = do_shortcode( "[bbboing]" );
		$this->assertStringStartsWith( "<p>A", $result );
		bw_trace2( $result );
	}

}

