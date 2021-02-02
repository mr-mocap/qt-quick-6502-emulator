# TODO & Feature List

## Near Term
------

### Processor
##### Implement BCD mode
Unit testing approach:  
Only test the BCD states themselves, leaving the other aspects of the instruction tested by other unit tests.
We will only have to test the instruction at a single location in memory and addressing mode for the various
expected BCD states, as we only want to test the adding capability itself.
Roll through the numbers for each digit once: 00 - 09 (first digit), (x)9 increments to (x+1)0.  99 increments to 00.

### Qt
* Flesh out the current Memory Page View widgets
    * Single-page view
        * Hex view
        * ASCII view
    * Single-page view with ability to change current page being viewed
        * Read-Only version
        * Read/Write version
        * Hex view
        * ASCII view
    * Disassembly view
    * Processor status view
* Use QML for one of the widgets as a demonstration of how to add QML widgets to a Qt app.

### Other
* Ability to save/restore the processor state

# Far Term
