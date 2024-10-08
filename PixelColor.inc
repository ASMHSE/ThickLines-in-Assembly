; ==================================================================================================
; Title:      PixelColor.inc
; Author:     Héctor S. Enrique
; Version:    1.0.0
; Purpose:    Thick Lines in Assembly.
; Notes:      Version 1.0.0, October 2024
;               - First release.
; ==================================================================================================

takecolor macro color, cc1, cc2, cc3
    mov eax , color
    and eax , 00000FFh
    mov cc1, eax
    mov eax , color
    and eax , 000FF00h
    shr eax, 8
    mov cc2, eax
    mov eax , color
    and eax , 0FF0000h
    shr eax, 16
    mov cc3, eax
endm

remakecolorC macro cc1, cc2, cc3
    fld   st
    fld   st
    fild  cc1
    fmul
    fistp c1 
    fimul cc2
    fistp c2 
    fimul cc3
    fistp c3
    mov   eax, c1
    shl   eax, 8
    add   eax, c2
    shl   eax, 8
    add   eax, c3
endm

setpixelM macro x1, y1
        push xdi
        mov eax , y1
        mov ecx , cdXSize
        shl ecx , 2
        mul ecx 
        add xdi , xax
        mov eax , x1
        shl eax , 2
        add xdi , xax
        .if fGT(r4, real4 ptr [xdi])
            fld  r4
            fstp real4 ptr [xdi]
        .endif
        pop xdi
endm

setpixelC macro x1, y1
        push xbx
        push xdi
        mov eax , y1
        mov ecx , cdXSize
        shl ecx , 2
        mul ecx 
        mov xdx , xax
        mov eax , x1
        shl eax , 2
        add xdx , xax
        add xdi, xdx
        add xbx, xdx
        .if fGT( real4 ptr [xdi], 0)
            fld  real4 ptr [xdi]
            remakecolorC cc1, cc2, cc3
            mov dword ptr [xbx], eax
            fldz
            fstp real4 ptr [xdi]
        .endif
        pop xdi
        pop xbx
endm

setpixelD macro x1, y1
        push xbx
        push xdi
        mov eax , y1
        mov ecx , cdXSize
        shl ecx , 2
        mul ecx 
        mov xdx , xax
        mov eax , x1
        shl eax , 2
        add xdx , xax
        add xdi, xdx
        add xbx, xdx
        .if fGT(r4, 0)
            fld  r4
            remakecolorC cc1, cc2, cc3
            mov dword ptr [xbx], eax
        .endif
        pop xdi
        pop xbx
endm
