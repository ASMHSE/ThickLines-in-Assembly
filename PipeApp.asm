; ==================================================================================================
; Title:      PipeApp.asm
; Author:     Héctor S. Enrique
; Version:    1.0.0
; Purpose:    Thick Lines in Assembly.
; Notes:      Version 1.0.0, October 2024
;               - First release.
; ==================================================================================================

;----------------------------------------------------------------------------  
;                                                                           -
;        Basic structure from Alfonso Víctor Caballero Hurtado               -          
;                             "Abre los Ojos a Win Gráficos" (2017)         - 
;----------------------------------------------------------------------------  

if 0
    
    ;  Masm32 SDK

include \masm32\include\masm32rt.inc

    OAC equ 0
    xax textequ <eax>
    xbx textequ <ebx>
    xcx textequ <ecx>
    xdx textequ <edx>
    xsi textequ <esi>
    xdi textequ <edi>

    POINTER textequ <dword>
    xword   textequ <dword>
else

    ;   ObjAsm

% include @Environ(OBJASM_PATH)\Code\Macros\Model.inc   ;Include & initialize standard modules
SysSetup WIN32, ANSI_STRING;, DEBUG(WND,INFO)            ;Load OOP files and OS related objects

    OAC equ 1

endif

include \masm32\macros\SmplMath\Math.inc
fSlvSelectBackEnd X86,FPU

include \masm32\macros\SmplMath\accs\macros.inc


        ; ——————————————————————————————————————————————————————————————————————————————————————————————————
        ; Macro:      CopiaStruct
        ; Purpose:    Output a specified wide string on the debug device.
        ; Arguments:  Arg1: Structure.
        ;             Arg2: Destination structure.
        ;             Arg3: Source structure.
        
        CopiaStruct macro tipo_de_objeto, ObjectDest, ObjectOrig
            
            push xsi
            
            cld
            mov xdi, &ObjectDest
            mov xsi, &ObjectOrig
            mov xcx , sizeof &tipo_de_objeto
            rep movsb
            pop xsi
        endm
                
        return macro val1
            ifnb <val1>
                mov xax, val1
            endif
            ret
        endm

    puntito struct
        xg      real8 0.0
        yg      real8 0.0
        valor   real8 0.0
        speed   real8 0.0
        first   xword  0
    puntito ends   
 
.const

    cdXPos       dd    128
    cdYPos       dd    128
    cdXSize      dd    640
    cdYSize      dd    400
    cdColFondo   dd      0   ;     // COLOR_BTNFACE + 1
    cdVBarTipo   dd      0
    cdVBtnTipo   dd   WS_VISIBLE or WS_SYSMENU or WS_MINIMIZEBOX
    cdMainIcon   dd    100
    cdIdTimer    dd   1001
    
    
.data
    todos  POINTER 0
    todos1 POINTER 0


    ultimo   puntito <?>
    temppunt puntito <?>

    ; Variables globales
    ife OAC
    hInstance HINSTANCE 0
    endif

    bi  BITMAPINFOHEADER  <sizeof BITMAPINFOHEADER, 640,-400,1,32,0,0,0,0,0,0>
    hMainWnd    POINTER 0
    bufDIBDC    POINTER 0
    pMainDIB    POINTER 0
    hBaseWnd    POINTER 0
    pBaseDIB    POINTER 0

    szClaseName db "ClasePintaA",0
    ife OAC
        szTitle     db "Thick Lines with MASM32 SDK", 0
    else
        szTitle     db "Thick Lines with ObjAsm-C.2", 0
    endif

    n_todos     dd 0
    n_todos1    dd 0
    n_final     dd 0

    iniciado dd FALSE

    flastx  real8 0.0
    flasty  real8 0.0

.code

start:

; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

    if OAC
        SysInit
        DbgClearAll
    else
        mov hInstance,   rv(GetModuleHandle, NULL)
    endif

    call Win_Main

    if OAC
        SysDone
    endif
    invoke ExitProcess,eax

    cargapuntito macro fx, fy 
        fSlv [xdx].puntito.xg = fx
        fSlv [xdx].puntito.yg = 355-fy
        add xdx, xcx
        add xax, 1
    endm

CargaLinea proc
    mov xdx, todos
    mov ecx, sizeof puntito
    mov eax, 0

    cargapuntito  20,  20
    cargapuntito 100, 100
    cargapuntito 150, 100
    cargapuntito 200, 125
    cargapuntito 200, 100
    cargapuntito 250, 150
    cargapuntito 300, 225
    cargapuntito 450,  50
    cargapuntito 475, 150
    cargapuntito 575, 120

    mov n_todos, eax

    mov xdx, todos1
    mov eax, 0

    cargapuntito  20,   340
    cargapuntito  200,  20
    cargapuntito  350,  120
    cargapuntito  500,  280
    cargapuntito  575,  280


    mov n_todos1, eax

    ret
CargaLinea endp

include pixelcolor.inc
include ColorBox.inc

ColorBox M,P
ColorBox C,P
;ColorBox D
ColorBox M,S
ColorBox C,S

include DoubleBresenham.inc

DoubleBresenham M,P
DoubleBresenham C,P
;DoubleBresenham D
DoubleBresenham M,S
DoubleBresenham C,S

CleanScreen proc
        mov ecx , 640*400           ; clean screen
        mov xdx, pMainDIB
    @loop1:
        mov eax, 0
        mov dword ptr [xdx], eax
        add xdx, @WordSize
        loop @loop1
    ret
CleanScreen endp

MuestraLinea proc uses xdi xbx w:real8, xcolor:dword
    local i : dword
    local tope : dword
    mov eax, xcolor
    bswap eax
    ror eax, 8
    mov xcolor,  eax

    mov xdi, pBaseDIB
    mov xbx, pMainDIB

    mov iniciado, FALSE

    ForLpD i, 0, n_todos
        imul xdx, xax, sizeof puntito
        mov xax, todos
        add xdx, xax
        .if iniciado == FALSE
            mov iniciado, TRUE
        .else
            push xdx
            fn DoubleBresenhamMP, [xdx].puntito.xg, [xdx].puntito.yg, w
            pop xdx
        .endif
        fSlv8 flastx = [xdx].puntito.xg
        fSlv8 flasty = [xdx].puntito.yg
    Next i
    ForLpD i, 0, n_todos
        imul xdx, xax, sizeof puntito
        mov xax, todos
        add xdx, xax
        fn ColorBoxMP, [xdx].puntito.xg, [xdx].puntito.yg, w
    Next i

    mov iniciado, FALSE
    ForLpD i, 0, n_todos
        imul xdx, xax, sizeof puntito
        mov xax, todos
        add xdx, xax
        .if iniciado == FALSE
            mov iniciado, TRUE
        .else
            push xdx
            fn DoubleBresenhamCP, [xdx].puntito.xg, [xdx].puntito.yg, w, xcolor
            pop xdx
        .endif
        fSlv8 flastx = [xdx].puntito.xg
        fSlv8 flasty = [xdx].puntito.yg
    Next i
    ForLpD i, 0, n_todos
        imul xdx, xax, sizeof puntito
        mov xax, todos
        add xdx, xax
        fn ColorBoxCP, [xdx].puntito.xg, [xdx].puntito.yg, w, xcolor
    Next i
    ret
MuestraLinea endp

MuestraLineaS proc uses xdi xbx w:real8, xcolor:dword
    local i : dword
    local tope : dword
;    local punterox: dword, punteroy : dword

    mov eax, xcolor
    bswap eax
    ror eax, 8
    mov xcolor,  eax
    
    mov xdi, pBaseDIB
    mov xbx, pMainDIB

    mov iniciado, FALSE

    ForLpD i, 0, n_todos1
        imul xdx, xax, sizeof puntito
        mov xax, todos1
        add xdx, xax
        .if iniciado == FALSE
            mov iniciado, TRUE
        .else
            push xdx
            fn DoubleBresenhamMS, [xdx].puntito.xg, [xdx].puntito.yg, w
            pop xdx
        .endif
        fSlv8 flastx = [xdx].puntito.xg
        fSlv8 flasty = [xdx].puntito.yg
    Next i
    ForLpD i, 0, n_todos1
        imul xdx, xax, sizeof puntito
        mov xax, todos1
        add xdx, xax
        fn ColorBoxMS, [xdx].puntito.xg, [xdx].puntito.yg, w
    Next i

    mov iniciado, FALSE
    ForLpD i, 0, n_todos1
        imul xdx, xax, sizeof puntito
        mov xax, todos1
        add xdx, xax
        .if iniciado == FALSE
            mov iniciado, TRUE
        .else
            push xdx
            fn DoubleBresenhamCS, [xdx].puntito.xg, [xdx].puntito.yg, w, xcolor
            pop xdx
        .endif
        fSlv8 flastx = [xdx].puntito.xg
        fSlv8 flasty = [xdx].puntito.yg
    Next i
    ForLpD i, 0, n_todos1
        imul xdx, xax, sizeof puntito
        mov xax, todos1
        add xdx, xax
        fn ColorBoxCS, [xdx].puntito.xg, [xdx].puntito.yg, w, xcolor
    Next i
    ret
MuestraLineaS endp

MainWndProc proc  hWnd:HWND, message:UINT, wParam:WPARAM, lParam:LPARAM

   local    hdc : HDC
   local    hMainDIB : HBITMAP
   local    hBaseDIB : HBITMAP
   local    ps : PAINTSTRUCT

   .switch message 

        .case WM_CHAR
     
            .if (wParam == VK_ESCAPE) 
                jmp wmDestroy;
            .endif
    
        .case WM_CREATE

             mov hdc, rv(GetDC ,hWnd)
             ; Crea un búfer dib para PintaObjeto. pMainDIB es un puntero a él
             mov bufDIBDC , rv(CreateCompatibleDC, hdc)
             mov hMainDIB , rv(CreateDIBSection, hdc, addr bi, DIB_RGB_COLORS, addr pMainDIB, NULL, 0);
             mov hBaseDIB , rv(CreateDIBSection, hdc, addr bi, DIB_RGB_COLORS, addr pBaseDIB, NULL, 0);
             fn SelectObject, bufDIBDC, hMainDIB
             fn ReleaseDC, hWnd, hdc     ;   // Libera device context
             fn CargaLinea

        .case WM_PAINT 
            mov hdc , rv(BeginPaint,hWnd, addr ps);

            fn CleanScreen
            fn MuestraLineaS, FP8(12.0), 0990099h
            fn MuestraLinea, FP8(12.0), 0FF00h

            fn BitBlt,hdc, 0, 0, cdXSize, cdYSize, bufDIBDC, 0, 0, SRCCOPY
            fn EndPaint,hWnd, ADDR ps

        .case WM_DESTROY 
            wmDestroy:
            fn GlobalFree, todos
            fn GlobalFree, todos1
            fn DeleteDC,bufDIBDC
            fn DeleteObject,hMainDIB
            fn DeleteObject,hBaseDIB
            fn DestroyWindow,hWnd
            fn PostQuitMessage,0

        .default
            return rvc(DefWindowProc,hWnd, message, wParam, lParam);
        .endsw
    return 0 
MainWndProc endp

Win_Main proc

    local msg :MSG ;
    local wndclass :WNDCLASS;
    local rctWnd:RECT
    local rctClient:RECT ;
    local ptDiff: POINT;    // Para calcular nuevo tamaño de Wnd


     fn GlobalAlloc, GMEM_FIXED, sizeof puntito *500000
     mov todos, xax   
     fn GlobalAlloc, GMEM_FIXED, sizeof puntito *500000
     mov todos1, xax   
     
    mov wndclass.style , CS_HREDRAW or CS_VREDRAW ;
    mov wndclass.lpfnWndProc, offset MainWndProc ;
    mov wndclass.cbClsExtra,0 ;
    mov wndclass.cbWndExtra, 0 ;
    mov wndclass.hbrBackground, NULL; //(HBRUSH) GetStockObject(LTGRAY_BRUSH);
    mov wndclass.lpszMenuName,  NULL ;
    mov wndclass.lpszClassName, offset szClaseName
    m2m wndclass.hInstance, hInstance; //GetModuleHandle (NULL) ;
    mov wndclass.hIcon  , rv( LoadIcon, hInstance, 10)
    mov wndclass.hCursor, rv(LoadCursor,NULL, IDC_ARROW)
     
    .if !rvc(RegisterClass,addr wndclass) 
        fn MessageBox,0,"This program requires Windows NT!","Error",MB_OK
        return 0 ;
     .endif
     
     invoke CreateWindowEx, WS_EX_APPWINDOW, addr szClaseName,\
                       addr szTitle, cdVBtnTipo, cdXPos, cdYPos,\
                       cdXSize, cdYSize, NULL, NULL, hInstance, NULL
     mov hMainWnd, eax
     .if !hMainWnd
         return 0
     .endif

     fnc MoveWindow, hMainWnd, cdXPos, cdYPos, cdXSize, cdYSize , TRUE
     fnc ShowWindow, hMainWnd, SW_SHOW
     fnc UpdateWindow, hMainWnd
 @@vuelta:    
    fn GetMessage, &msg, NULL, 0, 0 
    .if eax  
        fnc TranslateMessage, addr msg
        fnc DispatchMessage, addr msg
        jmp @@vuelta
     .endif
     return msg.wParam 
Win_Main endp
 
end start

