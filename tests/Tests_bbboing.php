<?php
/**
 * Tests bbboing.
 *
 * @copyright (C) Copyright Bobbing Wide 2016, 2017, 2020, 2024
 *
 *
 */
class Tests_bbboing extends WP_UnitTestCase {

	/**
	 * setUp is run for every test
	 */
	function setUp(): void {
		// parent::setUp();
		//do_action( "oik_add_shortcodes" );
		//echo "setUp run";
	}

	/**
	 * tearDown is run for every test
	 */
	function tearDown(): void {
		// parent::tearDown();
		//echo "tearDown run";
	}

	/**
	 * Demonstrate that bbboing is active
	 *
	 * If this fails it suggests that the plugin is not activated.
	 * Other tests are also expected to fail.
	 */
	function test_bbboing_loaded_action_performed() {
		$performed = did_action( "bbboing_loaded" );
		$this->assertEquals( 1, $performed );
	}

	/**
	 * Ensure that oik shortcodes can be run.
	 *
	 * There is a chance that oik_add_shortcodes has already been run more than once!
	 * eg Once by oik when it's detected a block based theme
	 * then later by a block based theme which was written before oik v4.14.0
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

