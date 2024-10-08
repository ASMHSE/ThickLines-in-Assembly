; ==================================================================================================
; Title:      DoubleBresenham.inc
; Author:     Héctor S. Enrique
; Version:    1.0.0
; Purpose:    Thick Lines in Assembly.
; Notes:      Version 1.0.0, October 2024
;               - First release.
; ==================================================================================================

    MAX_BRESEN_THICK equ 100

.data

    counter dword 0

    cutn dword 0
    cutx sdword MAX_BRESEN_THICK dup (0)
    cuty sdword MAX_BRESEN_THICK dup (0)
    cutnx dword 0
    cutxx sdword MAX_BRESEN_THICK dup (0)
    cutyx sdword MAX_BRESEN_THICK dup (0)
    cutny dword 0
    cutxy sdword MAX_BRESEN_THICK dup (0)
    cutyy sdword MAX_BRESEN_THICK dup (0)

.code

    primerparte macro
        fSlv a1 = (fy-flasty)
        fSlv c1a = (fx-flastx)
        fSlv ang1 = cos(atan2(a1,c1a))
        fSlv ang2 = sin(atan2(a1,c1a))
    endm

    draw_cut macro letter, kind
        ForLpD i, 0, cutn, eax
            fSlv xx = x0 + cutx[eax*4]
            fSlv yy = y0 + cuty[eax*4]
    
            fSlv c2a = (xx-fx)
            fSlv a2 = c2a / c1a* a1
            fSlv a3 = (yy - fy) - a2
            fSlv8 r = abs(ang1*a3)/(w/2)
    
            aa_pipe letter, kind, xx, yy
        Next i
    endm


    draw_cut2 macro letter, kind, a
    
        ForLp i, 0, cutn&a&
            fSlv xx = x0 + cutx&a&[eax*4]
            fSlv yy = y0 + cuty&a&[eax*4]
    
            fSlv c2a = (xx-fx)
            fSlv a2 = c2a / c1a* a1
            fSlv a3 = (yy - fy) - a2
            fSlv8 r = abs(ang1*a3)/(w/2)
    
            aa_pipe letter, kind, xx, yy
        Next i
    endm

  
process_cut proc uses esi edi ebx dx1:sdword,dy1:sdword
    local i :dword
    ;mov eax, dx1
    ;.if eax < dy1
        mov cutnx, 0
        DbgText "cut2a"
        mov esi, cutx
        mov cutxx, esi
        mov edi, cuty
        mov cutyx, edi
        ForLpD i, 1, cutn
            mov ecx, cutx[eax*4]
            mov edx, cuty[eax*4]
            .if edi != edx
                inc cutnx
                mov eax, cutnx
               mov cutxx[eax*4], ecx
                mov cutyx[eax*4], edx
            .else
                .if esi != ecx
                    mov eax, cutnx
                    mov cutxx[eax*4], ecx
                    mov cutyx[eax*4], edx
                .endif
            .endif
            mov esi, ecx
            mov edi, edx
        Next i
    ;.else
        mov cutny, 0
        mov esi, cutx
        mov cutxy, esi
        mov edi, cuty
        mov cutyy, edi
        ForLpD i, 1, cutn
            mov ecx, cutx[eax*4]
            mov edx, cuty[eax*4]
            .if esi != ecx
                inc cutny
                mov eax, cutny
                mov cutxy[eax*4], ecx
                mov cutyy[eax*4], edx
            .else
                .if edi == edx
                    mov eax, cutny
                    mov cutxy[eax*4], ecx
                    mov cutyy[eax*4], edx
                .endif
            .endif
            mov esi, ecx
            mov edi, edx
        Next i
    ;.endif
    ret
process_cut endp

    prep_bresen_cut macro eje
        mov eax, eje&6
        sub eax, eje&5
        mov d&eje&1, eax
        .if sdword ptr d&eje&1 < 0
            fSlv d&eje&1 = -d&eje&1
            mov step&eje, -1
        .else
            mov step&eje, 1
        .endif
    endm

    error_bresen macro a, b
        mov eax, d&a&1
        shl eax, 1
        sub eax, d&b&1
        mov d, eax  
        mov eax, d&a&1
        shl eax, 1
        mov e, eax           
        mov eax, d&a&1
        sub eax, d&b&1
        shl eax, 1
        mov _ne, eax
    endm

    loop_bresen_cut macro a, b, c, k
        mov eax, a&5
        mov a&7, eax
      entra&a&:
            mov ecx, cutn
            mov eax, x&c
            sub eax, x0
            mov cutx[ecx*4],eax 
            mov eax, y&k
            sub eax, y0
            mov cuty[ecx*4], eax
            inc cutn
            mov eax, d            
            .if sdword ptr eax <= 0
                add eax, e
            .else
                add eax, _ne
                mov ecx, &b&5
                add ecx, step&b&
                mov &b&5, ecx
            .endif
            mov d, eax 
        mov eax, &a&7
        cmp eax, &a&6
        je sale
        add eax, step&a
        mov a&7, eax
        jmp entra&a 
    endm

store_cut proc x0:sdword,y0:sdword,x5:sdword,y5:sdword,x6:sdword,y6:sdword
    local dx1:sdword,dy1:sdword
    local d:sdword, e:sdword, _ne:sdword
    local x7:dword, y7:sdword
    local stepx:sdword, stepy:sdword

    fSlv cutn = 0

    prep_bresen_cut x
    prep_bresen_cut y

    mov eax, dx1
    .if eax > dy1
        error_bresen y, x
        loop_bresen_cut x, y, 7, 5
    .else
        error_bresen x, y
        loop_bresen_cut y, x, 5, 7
    .endif
  sale:  
    ret
store_cut endp

perpendicular proc x0:sdword, y0:sdword, dx1:sdword, dy1:sdword, w:real8
    local pendiente : real8, constant: real8
    local r2:real8, y2 : real8
    local x3:sdword, y3:sdword, x4:sdword, y4:sdword
    local dx2: sdword, dy2:sdword

    fSlv dx2 = abs(dx1)
    fSlv dy2 = abs(dy1)
    fSlv pendiente = -dx1/dy1
    fSlv constant = y0 - pendiente * x0
    fSlv y2 = pendiente*(x0+1) + constant

    fSlv8 r2 = (1+(y0-y2)^2)^0.5
    fSlv x3 = (w/r2)/2

    fSlv x4 = x0 + x3
    fSlv x3 = x0 - x3
    fSlv y3 = pendiente * x3 + constant
    fSlv y4 = pendiente * x4 + constant

    invoke store_cut, x0, y0, x3, y3, x4, y4
    invoke process_cut, dx2, dy2
    ret
perpendicular endp

        vertical macro letter, kind
            fSlv x3 = down(x0 - w/2)
            fSlv x4 = ceil(x0 + w/2)
            ForLp i, x3, x4
                fSlv8 r = abs(i-fx)/(w/2)
                aa_pipe letter, kind, i, y0
            Next i
        endm
    
        horizontal macro letter, kind
            fSlv y3 = down(y0 - w/2)
            fSlv y4 = ceil(y0 + w/2)
            ForLp i, y3, y4
                fSlv8 r = abs(i-fy)/(w/2)
                aa_pipe letter, kind, x0, i
            Next i
        endm 

    rect_bresen macro nombre, letter, kind, b, a
        %.if d&b&1 == 0
          entra_&a&: 
            nombre letter, kind
            mov eax, &a&0
            cmp eax, &a&1
            je sale_&a&
            add eax, step&a&
            mov &a&0, eax
            jmp entra_&a& 
          sale_&a&: 
            ret
        .endif
    endm
    prep_bresen_main macro eje
        mov eax, eje&1
        sub eax, eje&0
        mov d&eje&1, eax
        mov d&eje&0, eax
        .if sdword ptr d&eje&1 < 0
            neg d&eje&1; = -d&eje&1
            mov step&eje, -1
        .else
            mov step&eje, 1
        .endif
    endm

    loop_bresen_main macro letter, kind, a, b 
      entra&a&: 
        draw_cut letter, kind
        mov eax, d 
        .if sdword ptr eax <= 0
            add eax, e
            mov d, eax
        .else
            add eax, _ne
            mov d, eax
            mov eax, &b&0
            add eax,  step&b&
            mov &b&0, eax
            draw_cut2 letter, kind, &b
        .endif
        mov eax, &a&0
        cmp eax, &a&1
        je sale
        add eax, step&a&
        mov &a&0, eax
        jmp entra&a& 
    endm

DoubleBresenham macro letter, kind

        colorea = 1
        brillo = 1
        ifidn <letter>, <M>
            colorea = 0
        endif
        ifidn <letter>, <C>
            brillo = 0
        endif

    DoubleBresenhamProc TEXTEQU <DoubleBresenham&letter&kind proc fx:real8, fy:real8, w:real8> 
    if colorea
        DoubleBresenhamProc CATSTR DoubleBresenhamProc, < , xcolor:dword >
    endif
    %DoubleBresenhamProc

    local x0:sdword, y0:sdword
    local x1:sdword, y1:sdword
    local dx1:sdword, dy1:sdword
    local dx0:sdword, dy0:sdword
    local d:sdword, e:sdword, _ne:sdword
    local stepx:sdword, stepy:sdword

    local i : sdword
    local x3:sdword, y3:sdword
    local x4:sdword, y4:sdword
    local xx:sdword, yy:sdword

    local r:real8, rx :real8
    local a1 :real8, a2 :real8, a3 :real8
    local c1a:real8, c2a:real8, d1 :real8
    local b1 :real8

    local coef:real8,ang1:real8, ang2:real8

    local fSlvTLS(,17)

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
    
        takecolor xcolor, cc1, cc2, cc3        ; running
    endif    

    fSlv x1 = fx    
    fSlv y1 = fy    
    fSlv x0 = flastx    
    fSlv y0 = flasty    

    prep_bresen_main x
    prep_bresen_main y

    rect_bresen vertical, letter, kind, x, y
    rect_bresen horizontal, letter, kind, y, x

    invoke perpendicular, x1, y1, dx0, dy0, w

    primerparte

    mov eax, dx1
    .if eax > dy1
        error_bresen y, x
        loop_bresen_main letter, kind, x, y
    .else
        error_bresen x, y
        loop_bresen_main letter, kind, y, x
    .endif
  sale:
    ret
DoubleBresenham&letter&kind endp    

endm