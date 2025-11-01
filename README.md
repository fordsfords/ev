# ev
A low-overhead event recording subsystem for debugging.

## Table of contents

<!-- mdtoc-start -->
&bull; [ev](#ev)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Table of contents](#table-of-contents)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Introduction](#introduction)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [License](#license)  
<!-- TOC created by '../mdtoc/mdtoc.pl README.md' (see https://github.com/fordsfords/mdtoc) -->
<!-- mdtoc-end -->


## Introduction

Debugging multi-threaded code can be difficult,
espicially tracking down intermittent race conditions.
A typical approach is to add print statements to determine the
exact sequence of events,
but print statements themselves change the timing of the code,
changing the sequence of events.

The "ev" module lets you determine the exact sequence of events
with minimal impact on the timing.
Instead of adding print statements, you add "ev_write()" statements.
It uses a circular buffer to save the events for later analysis.


## License

I want there to be NO barriers to using this code, so I am releasing it to the public domain.  But "public domain" does not have an internationally agreed upon definition, so I use CC0:

This work is dedicated to the public domain under CC0 1.0 Universal:
http://creativecommons.org/publicdomain/zero/1.0/

To the extent possible under law, Steven Ford has waived all copyright
and related or neighboring rights to this work. In other words, you can 
use this code for any purpose without any restrictions.
This work is published from: United States.
Project home: https://github.com/fordsfords/ev
