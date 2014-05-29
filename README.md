rpos
====

Research Project Operating System.

Just playing around with systems programming.

Required Tools
===

At he current phase of this system, you will need to build a generic C cross compiler.

[http://wiki.osdev.org/GCC_Cross-Compiler GCC_Cross-Compiler] is a good start

At a future time we will build a OS Specific Toolchain but right now, i dont know how calling convention is going to work, so we will leave it at this for now.

Configuration
===

After you have built your cross compiler, you need to tell the build system where the toolchain is.
Update the appropriate values in the config.mk file.

