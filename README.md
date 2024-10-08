# An Anti-Alised Thick Line: The Double Bresenham's Line



[![](https://img.shields.io/badge/Assembler-UASM%20v2.52-green.svg?style=flat-square&logo=visual-studio-code&logoColor=white&colorB=1CC887)](http://www.terraspace.co.uk/uasm.html) 

Framework : ObjAsm C.2  - masters : [Main Developer](https://github.com/ObjAsm/ObjAsm-C.2) , [Fork](https://github.com/ASMHSE/ObjAsm-C.2/tree/master)
                        - working version [6 march 2024](https://github.com/ASMHSE/ObjAsm-C.2/tree/patch-2)   

This line use 2 times Bresenham algorithm: one for the line itself and other to calculate extreme lines, wich are prependicular to the main line.

Some remarks:

- The width from this process is the true intended width, not cuts width like in some other thick lines.

- To connect lines an additional element is used.

- Lines drawing and connections happen in a buffer screen and you just can see the result.

- Compact version (for development) use UASM assembler builded with more macros levels capacity (I hope unroll code ore other assemblers after additional tests).

Any sugestion or improvement is welcome!

