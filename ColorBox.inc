; ==================================================================================================
; Title:      ColorBox.inc
; Author:     Héctor S. Enrique
; Version:    1.0.0
; Purpose:    Thick Lines in Assembly.
; Notes:      Version 1.0.0, October 2024
;               - First release.
; ==================================================================================================

    aa_pipe macro letter, kind, _x, _y
        local @sale2
        ifidn <kind>,<S>
            ;echo is solid
                .if fLT(r, 0.9)
                    fld1
                    fstp r4
                    jmp @sale2 
                .endif    
                .if fGE(r, 1.1)
                    fldz
                    fstp r4                 
                    jmp @sale2 
                .endif
                fSlv8 rx = (r-0.9) / (1.1-0.9)
                fSlv r4 = r*(1-(rx*rx) * (3-2*rx))
            @sale2:

            setpixel&letter &_x, &_y
        else
            .if fLT(r, 1.0)
                ifidn <letter>, <M>
                    fSlv8 r4 = (1-r)^0.5
                endif
                ifidn <letter>, <D>
                    fSlv8 r4 = (1-r)^0.5
    
                endif
                setpixel&letter &_x, &_y
            .endif
        endif
    endm

ColorBox macro letter, kind

        colorea = 1
        brillo = 1
        ifidn <letter>, <M>
            colorea = 0
        endif
        ifidn <letter>, <C>
            brillo = 0
        endif

    ColorBoxProc TEXTEQU <ColorBox&letter&kind proc fx:real8, fy:real8, w:real8> 
    if colorea
        ColorBoxProc CATSTR ColorBoxProc, < , xcolor:dword >
    endif
    %ColorBoxProc

        local i  :dword, j  :dword
        local x3  :dword, y3  :dword
        local x4  :dword, y4  :dword

        local x1 :dword, y1 :dword
        local r  :real8 , rx :real8

        
        local fSlvTLS(,24)

        if brillo
            local r4:real4
        else
            ifidn <kind>,<S>
                local r4:real4
            endif
        endif
        if colorea
            local c1:dword, c2:dword,c3: dword
            local cc1:dword, cc2:dword,cc3: dword
        
            takecolor xcolor, cc1, cc2, cc3
        endif    

        fSlv x3 = down(fx - w/2)   
        fSlv y3 = down(fy - w/2)   
        fSlv x4 = ceil(fx + w/2)   
        fSlv y4 = ceil(fy + w/2)   
    
        ForLp i, x3, x4, eax
            ForLp j, y3, y4, eax
                fSlv8 r = ((i-fx)^2+(j-fy)^2)^0.5/(w/2)
                aa_pipe letter, kind, i, j  
            Next j
        Next i

        ret
    ColorBox&letter&kind endp
endm

;-------------------------------------------------------------------------------

