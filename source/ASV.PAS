{***********************************
 * ASV - Application Services v1.1 *
 * Copyright (c)1990 by SoftDesign *
 *  written by Thomas Much         *
 ***********************************
 * entwickelt am:     24.05.1990   *
 * letztes Update am: 28.10.1990   *
 ***********************************}

unit asv;
{$A+,B-,D-,E-,F+,I-,L-,N-,O+,R-,S-,V+}

interface

  const _magic     = 18011972;
        _asv       = 1.1;      _wmax      = 99;
        _bmax      = 14;       _imax      = 9;
        M_EXIT     = 255;
        M_OK       = 0;        M_HELP     = 254;
        M_NOP      = -1;       H_PULL     = 0;
        M_CANCEL   = -2;       H_POP      = 1;
        M_ERROR    = -10;      H_DIAL     = 2;
        E_NFOUND   = -11;      M_DIS      = 253;
        E_NREENT   = -12;
        E_NDRAW    = -13;      KEY_NONE   = 0;
        E_MOUTSCR  = -14;      KEY_NORM   = 127;
        E_MUSED    = -15;      KEY_SPEC   = 27;
        E_NINIT    = -16;
        E_NBUT     = -17;      P_OFFS     = 123;
        E_DSTRUC   = -18;
        E_DTBIG    = -19;
        E_MSTRUC   = -20;
        RSHIFT     = 1;        LSHIFT     = 2;
        CTRL       = 4;        ALT        = 8;
        SCRLLOCK   = 16;       NUMLOCK    = 32;
        CAPSLOCK   = 64;       INS        = 128;
        L_USA      = 1;        PC         = 0;
        L_FRG      = 2;        XT         = 1;
        L_GB       = 8;        AT         = 3;
        A_NCHECKED = 1;        A_CHECKED  = 2;
        A_ENABLED  = 3;        A_DISABLED = 4;
        M_LEFT     = 1;        M_MID      = 4;        M_RIGHT    = 2;
        K_F1       = 59;       K_F2       = 60;       K_INS      = 82;
        K_F3       = 61;       K_F4       = 62;       K_DEL      = 83;
        K_F5       = 63;       K_F6       = 64;       K_HOME     = 71;
        K_F7       = 65;       K_F8       = 66;       K_END      = 79;
        K_F9       = 67;       K_F10      = 68;       K_PGUP     = 73;
        K_ESC      = 27;       K_TAB      = 9;        K_PGDN     = 81;
        K_BS       = 8;        K_RET      = 13;       K_UP       = 72;
        K_LF       = 10;       K_FF       = 12;       K_DOWN     = 80;
                               K_LEFT     = 75;       K_RIGHT    = 77;

  type vtype = record
                 case integer of
                   0  : (b : byte);
                   1  : (si : shortint);
                   2  : (i : integer);
                   3  : (w : word);
                   4  : (l : longint);
                   5  : (bo : boolean);
                   6  : (c : char);
                   7  : (s : string);
                   8  : (r : real);
                   9  : (sr : single);
                   10 : (d : double);
                   11 : (e : extended);
                   12 : (co : comp);
               end;

       pvtype = procedure(wnum : byte);

  var work        : array [0.._wmax] of string;
      intout      : array [0.._imax] of integer;
      byteout     : array [0.._bmax] of byte;
      void        : vtype;

  procedure hide;
  function left(teil : string; anz : byte) : string;
  function right(teil : string; anz : byte) : string;
  function space(anz : byte) : string;
  function strg(teil : string; anz : byte) : string;
  function kbshift(bits : byte) : boolean;
  procedure showmouse;
  procedure hidemouse;
  function mousex : byte;
  function mousey : byte;
  function mouseb : byte;
  procedure getmouse(var mx,my,mb : byte);
  procedure setmouse(mx,my : byte);
  procedure defmouse(smask,cmask : word);
  function attr(ink,pap : byte) : byte;
  function exist(path : string) : boolean;
  procedure cls(att : byte);
  function dtext(dx,dy : byte; dt : string) : string;
  function dframe(dx,dy,db,dh : byte; dt : string) : string;
  function dialog : integer;
  function form_alert(title,msg,but : string; def : byte) : integer;
  procedure form_error(num : integer);
  procedure form_seterrl(cc : integer);
  procedure menu_init;
  procedure menu_draw;
  function menu_watch : integer;
  procedure menu_kill;
  procedure menu_add(wrk : string);
  procedure menu_delete(num : byte);
  procedure menu_rename(num : byte; wrk : string);
  function menu_pop(wo : byte) : integer;
  procedure menu_setc(was,saw,has,haw,dis,shdw : byte);
  procedure menu_resetc;
  procedure scrforce(anzx,anzy : byte; avcol : boolean);
  procedure menu_setp(procv : pvtype);
  procedure menu_resetp;
  procedure menu_attr(num,attrs : byte);
  function menu_stat(num : byte) : byte;
  function menu_text(num : byte) : string;
  procedure menu_inquire;
  function _scrx : byte;
  function _scry : byte;
  function color : boolean;
  function mouse : boolean;
  function mvisible : boolean;
  procedure mouse_sets(sx,sy : integer);
  procedure mouse_resets;
  procedure mouse_sett(dst : integer);
  procedure mouse_resett;
  function asverror : integer;


implementation

  uses dos,crt;

  const _mmax = 19; _emin = -20; _viormax = 2239;

  var mbegin,mxpos                 : array [0.._mmax] of integer;
      mecurr                       : array [0.._mmax] of byte;
      mhkey                        : array [0.._wmax] of char;
      mhpos,attrib                 : array [0.._wmax] of byte;
      mwork                        : array [0.._wmax] of string[30];
      dwork                        : array [0.._wmax] of string;
      bxpos                        : array [0..19] of byte;
      max_entry,m_max,m_stat,m_num : integer;
      anzline,anzcol,asvierror     : integer;
      ccode,menn,loff              : integer;
      farbe,mausda,mausvis,menk    : boolean;
      vioseg                       : word;
      vioram                       : array [0.._viormax] of word;
      titleram                     : array [0..169] of word;
      weissaufschwarz,disabled,sc  : byte;
      hellaufschwarz,shadow,dhkpos : byte;
      schwarzaufweiss,hellaufweiss : byte;
      manzbut,menx,meny            : byte;
      m_pcall                      : pvtype;
      dhkstr                       : string[30];
      sdummy                       : string;
      exitsave                     : pointer;
      regs                         : registers;


procedure hide;

  begin
    gotoxy(anzcol,anzline);
  end;


function left(teil : string; anz : byte) : string;

  begin
    left:=copy(teil,1,anz);
  end;


function right(teil : string; anz : byte) : string;

  begin
    if anz>length(teil) then anz:=length(teil);
    right:=copy(teil,succ(length(teil)-anz),anz)
  end;


function space(anz : byte) : string;

  begin
    sdummy:='';
    if anz>0 then for sc:=1 to anz do sdummy:=sdummy+' ';
    space:=sdummy;
  end;


function strg(teil : string; anz : byte) : string;

  begin
    sdummy:='';
    if anz>0 then for sc:=1 to anz do sdummy:=sdummy+teil;
    strg:=sdummy;
  end;


function kbshift(bits : byte) : boolean;

  begin
    regs.ah:=2;
    intr($16,regs);
    if (regs.al and bits)=bits then kbshift:=true
      else kbshift:=false;
  end;


procedure showmouse;

  begin
    if (m_stat<>2) and mausda then
      begin
        regs.ax:=1;
        intr($33,regs);
        mausvis:=true;
      end;
  end;


procedure hidemouse;

  begin
    if (m_stat<>2) and mausda and mausvis then
      begin
        regs.ax:=2;
        intr($33,regs);
        mausvis:=false;
      end;
  end;


function mousex : byte;

  begin
    if mausda then
      begin
        regs.ax:=3;
        intr($33,regs);
        mousex:=succ(regs.cx shr 3);
      end
    else mousex:=0;
  end;


function mousey : byte;

  begin
    if mausda then
      begin
        regs.ax:=3;
        intr($33,regs);
        mousey:=succ(regs.dx shr 3);
      end
    else mousey:=0;
  end;


function mouseb : byte;

  begin
    if mausda then
      begin
        regs.ax:=3;
        intr($33,regs);
        mouseb:=regs.bx;
      end
    else mouseb:=0;
  end;


procedure getmouse(var mx,my,mb : byte);

  begin
    if mausda then
      begin
        regs.ax:=3;
        intr($33,regs);
        mx:=succ(regs.cx shr 3);
        my:=succ(regs.dx shr 3);
        mb:=regs.bx;
      end
    else
      begin
        mx:=0;  my:=0;  mb:=0;
      end;
  end;


procedure setmouse(mx,my : byte);

  begin
    if mausda then
      begin
        if mx<1 then mx:=1;
        if my<1 then my:=1;
        regs.ax:=4;
        regs.cx:=pred(mx) shl 3;
        regs.dx:=pred(my) shl 3;
        intr($33,regs);
      end;
  end;


procedure defmouse(smask,cmask : word);

  begin
    if mausda then
      begin
        regs.ax:=10;
        regs.bx:=0;
        regs.cx:=smask;
        regs.dx:=cmask;
        intr($33,regs);
      end;
  end;


function attr(ink,pap : byte) : byte;

  begin
    attr:=((pap and 15) shl 4)+(ink and 15);
  end;


function exist(path : string) : boolean;
  var pdata : searchrec;

  begin
    findfirst(path,0,pdata);
    if doserror=0 then exist:=true
      else exist:=false;
  end;


procedure cls(att : byte);
  var catt,vofs : word;
      q,start   : integer;

  begin
    hidemouse;
    catt:=(att shl 8)+32;
    if m_stat<=0 then
      begin
        vofs:=0;
        start:=0;
      end
    else
      begin
        vofs:=anzcol shl 1;
        start:=anzcol;
      end;
    for q:=start to pred(anzcol*anzline) do
      begin
        memw[vioseg:vofs]:=catt;
        inc(vofs,2);
      end;
    showmouse;
  end;


procedure setattr(tx,ty,art,len : byte);
  var vofs,q : word;

  begin
    vofs:=pred(ty)*loff+pred(tx)*2+1;
    for q:=1 to len do
      begin
        mem[vioseg:vofs]:=art;
        inc(vofs,2);
      end;
  end;


function dtext(dx,dy : byte; dt : string) : string;

  begin
    dtext:=chr(dx)+chr(dy)+dt;
  end;


function dframe(dx,dy,db,dh : byte; dt : string) : string;

  begin
    dframe:=#255#0+chr(dx)+chr(dy)+chr(db)+chr(dh)+dt;
  end;


procedure but_invert(def,bdw,bypos,was : byte);
  var q : byte;

  begin
    setattr(bxpos[def],bypos,was,1);
    setattr(succ(bxpos[def]+length(dwork[bdw+def])),bypos,was,1);
    q:=1;
    while (dwork[bdw+def,q]=' ') and (q<length(dwork[bdw+def])) do inc(q);
    gotoxy(bxpos[def]+q,bypos);
  end;


procedure get_dhotkey(num : byte);
  var hpos  : byte;
      dummy : string[1];

  begin
    hpos:=pos('#',dwork[num]);
    if hpos=length(dwork[num]) then hpos:=0;
    dhkpos:=hpos;
    if hpos>0 then
      begin
        dummy:=dwork[num,succ(hpos)];
        dhkstr:=upcase(dummy[1])+dhkstr;
        dwork[num]:=left(dwork[num],pred(hpos))+right(dwork[num],length(dwork[num])-hpos);
      end
    else dhkstr:=#0+dhkstr;
  end;


procedure mp_dis(msg : byte; mval : integer);

  begin
    m_stat:=2;
    m_pcall(msg);
    m_stat:=mval;
  end;


function dialog_do : integer;
  label derrjmp;

  var xp,yp,brt,hhe,dummy,owx,owy,h,b,cflag : byte;
      dpos,oms,wx,wy,wb,xflag,fb,fh,fy,fyc  : byte;
      taste                                 : char;
      vrnum,vofs,q,banz,bdw,def,bypos,flag  : integer;
      spstr                                 : string[200];

  begin
    if m_stat<>2 then
      begin
        owx:=wherex;
        owy:=wherey;
        dummy:=textattr;
        if length(dwork[0])<6 then
          begin
            dialog_do:=E_DSTRUC;
            exit;
          end;
        xp:=ord(dwork[0,1]);
        yp:=ord(dwork[0,2]);
        brt:=ord(dwork[0,5]);
        hhe:=ord(dwork[0,6]);
        if xp=255 then xp:=succ((anzcol-brt) shr 1);
        if yp=255 then yp:=succ((anzline shr 1)-(hhe shr 1));
        if xp<1 then xp:=1;
        if yp<1 then yp:=1;
        if succ(brt)*succ(hhe)>_viormax then
          begin
            dialog_do:=E_DTBIG;
            exit;
          end;
        if farbe then cflag:=0 else cflag:=1;
        if (xp+brt-cflag>anzcol) or (yp+hhe>anzline) or (brt<2) or (hhe<3) then
          begin
            dialog_do:=E_MOUTSCR;
            exit;
          end;
        hidemouse;
        q:=pred(yp)*loff+pred(xp)*2;
        vrnum:=0;
        for h:=0 to hhe do
          begin
            vofs:=q;
            for b:=0 to brt do
              begin
                vioram[vrnum]:=memw[vioseg:vofs];
                inc(vrnum);
                inc(vofs,2);
              end;
            inc(q,loff);
          end;
        textattr:=schwarzaufweiss;
        gotoxy(xp,yp);
        write('⁄',strg('ƒ',brt-2),'ø');
        spstr:='≥'+space(brt-2)+'≥';
        bypos:=yp+hhe-2;
        for q:=yp+1 to bypos do
          begin
            gotoxy(xp,q);
            write(spstr);
          end;
        gotoxy(xp,pred(yp+hhe));
        write('¿',strg('ƒ',brt-2),'Ÿ');
        if farbe then
          begin
            for q:=yp+1 to yp+hhe do setattr(xp+brt,q,shadow,1);
            setattr(succ(xp),yp+hhe,shadow,brt);
          end;
        if length(dwork[1])>0 then
          begin
            gotoxy(pred(xp+(brt shr 1)-(length(dwork[1]) shr 1)),yp);
            write(' ',dwork[1],' ');
          end;
        q:=2;
        while length(dwork[q])>0 do
          begin
            if hhe>4 then
              begin
                if length(dwork[q])<2 then
                  begin
                    flag:=E_DSTRUC;
                    showmouse;
                    goto derrjmp;
                  end
                else
                  begin
                    xflag:=ord(dwork[q,1]);
                    if xflag=255 then
                      begin
                        fb:=ord(dwork[q,5]);
                        fh:=ord(dwork[q,6]);
                        if (fb>1) and (fh>1) then
                          begin
                            xflag:=xp+ord(dwork[q,3]);
                            fy:=yp+ord(dwork[q,4]);
                            gotoxy(xflag,fy);
                            write('⁄',strg('ƒ',fb-2),'ø');
                            gotoxy(xflag,pred(fy+fh));
                            write('¿',strg('ƒ',fb-2),'Ÿ');
                            if (length(dwork[q])>6) and (fb>4) then
                              begin
                                cflag:=xflag+(fb shr 1)-((length(dwork[q])-4) shr 1);
                                if ((fb and 254)=fb) and ((length(dwork[q]) and 254)<>length(dwork[q])) then dec(cflag);
                                gotoxy(cflag,fy);
                                write(' ',right(dwork[q],length(dwork[q])-6),' ');
                              end;
                            if fh>2 then
                              for fyc:=0 to fh-3 do
                                begin
                                  inc(fy);
                                  gotoxy(xflag,fy);
                                  write('≥');
                                  gotoxy(pred(xflag+fb),fy);
                                  write('≥');
                                end;
                          end;
                      end
                    else
                      begin
                        gotoxy(xp+xflag,yp+ord(dwork[q,2]));
                        write(right(dwork[q],length(dwork[q])-2));
                      end;
                  end;
              end;
            inc(q);
          end;
        inc(q);
        bdw:=q;
        banz:=-1;
        while length(dwork[q])>0 do
          begin
            inc(banz);
            inc(q);
          end;
        if banz=-1 then
          begin
            flag:=E_NBUT;
            showmouse;
            goto derrjmp;
          end;
        bxpos[succ(banz)]:=pred(xp+brt);
        dhkstr:='';
        for q:=banz downto 0 do
          begin
            get_dhotkey(bdw+q);
            bxpos[q]:=bxpos[succ(q)]-length(dwork[bdw+q])-4;
            gotoxy(bxpos[q],bypos);
            write('<',dwork[bdw+q],'>');
            if dhkpos>0 then setattr(bxpos[q]+dhkpos,bypos,hellaufweiss,1);
          end;
        if hhe>3 then
          begin
            gotoxy(xp,yp+hhe-3);
            write('√',strg('ƒ',brt-2),'¥');
          end;
        def:=ord(dwork[0,3]);
        if def>banz then def:=0;
        but_invert(def,bdw,bypos,hellaufweiss);
        textattr:=dummy;
        flag:=M_NOP;
        showmouse;
        repeat
          getmouse(wx,wy,wb);
          if (wb and M_LEFT)=M_LEFT then if wy=bypos then
            for q:=0 to banz do if (wx>=bxpos[q]) and (wx<=succ(bxpos[q]+length(dwork[bdw+q]))) then flag:=q;
          if keypressed then
            begin
              taste:=readkey;
              if keypressed then
                begin
                  taste:=readkey;
                  case taste of
                    #15: begin
                           hidemouse;
                           but_invert(def,bdw,bypos,schwarzaufweiss);
                           dec(def);
                           if def<0 then def:=banz;
                           but_invert(def,bdw,bypos,hellaufweiss);
                           showmouse;
                         end;
                    #59: begin
                           hidemouse;
                           oms:=m_stat;
                           byteout[2]:=H_DIAL;
                           if length(dwork[0])>6 then byteout[3]:=ord(dwork[0,7])
                             else byteout[3]:=0;
                           mp_dis(M_HELP,oms);
                           but_invert(def,bdw,bypos,hellaufweiss);
                           showmouse;
                         end;
                  end;
                end
              else
                case taste of
                  #9: begin
                        hidemouse;
                        but_invert(def,bdw,bypos,schwarzaufweiss);
                        inc(def);
                        if def>banz then def:=0;
                        but_invert(def,bdw,bypos,hellaufweiss);
                        showmouse;
                      end;
                  #13: flag:=def;
                  #27: flag:=M_CANCEL;
                else
                  begin
                    dpos:=pos(upcase(taste),dhkstr);
                    if (dpos>0) and (dpos<=succ(banz)) then flag:=pred(dpos);
                  end;
                end;
            end;
        until flag<>M_NOP;
        derrjmp:
        hidemouse;
        q:=pred(yp)*loff+pred(xp)*2;
        vrnum:=0;
        for h:=0 to hhe do
          begin
            vofs:=q;
            for b:=0 to brt do
              begin
                memw[vioseg:vofs]:=vioram[vrnum];
                inc(vrnum);
                inc(vofs,2);
              end;
            inc(q,loff);
          end;
        textattr:=dummy;
        gotoxy(owx,owy);
        repeat until mouseb=0;
        showmouse;
        dialog_do:=flag;
      end
    else dialog_do:=E_NREENT;
  end;


function dialog : integer;
  var q : integer;

  begin
    for q:=0 to _wmax do dwork[q]:=work[q];
    dialog:=dialog_do;
  end;


function form_alert(title,msg,but : string; def : byte) : integer;
  var q,w,c,apos,bm : integer;

  begin
    dwork[0]:=#255#255+chr(def)+#0;
    dwork[1]:=title;
    q:=2;
    while (pos('|',msg)>0) and (pos('|',msg)<=length(msg)) do
      begin
        apos:=pos('|',msg);
        dwork[q]:=#3+chr(q)+left(msg,pred(apos));
        msg:=right(msg,length(msg)-apos);
        inc(q);
      end;
    if (q>2) or (length(msg)>0) then
      begin
        dwork[q]:=#3+chr(q)+msg;
        inc(q);
      end;
    dwork[q]:='';
    inc(q);
    w:=q;
    while (pos('|',but)>0) and (pos('|',but)<=length(but)) do
      begin
        apos:=pos('|',but);
        dwork[q]:=left(but,pred(apos));
        but:=right(but,length(but)-apos);
        inc(q);
      end;
    if (q>w) or (length(but)>0) then
      begin
        dwork[q]:=but;
        inc(q);
      end;
    if q=w then
      begin
        form_alert:=E_NBUT;
        exit;
      end;
    dwork[q]:='';
    dwork[succ(q)]:='';
    bm:=0;
    apos:=0;
    if w>3 then
      for c:=2 to w-2 do if length(dwork[c])>bm then bm:=length(dwork[c]);
    for c:=w to pred(q) do inc(apos,length(dwork[c])+2);
    inc(apos,succ(pred(q)-w));
    if apos>bm then bm:=apos;
    dwork[0]:=dwork[0]+chr(bm+4);
    if w=3 then dwork[0]:=dwork[0]+#3
      else dwork[0]:=dwork[0]+chr(w+3);
    dwork[0]:=dwork[0]+chr(byteout[4]);
    form_alert:=dialog_do;
  end;


procedure form_error(num : integer);
  const errmsgd : array [0..11] of string[31] =
                  ('Unbekannter Fehler',
                   'Allgemeiner Fehler',
                   'Resource-Datei nicht gefunden',
                   'Doppelter Aufruf nicht mîglich',
                   'MenÅ noch nicht gezeichnet',
                   'MenÅ au·erhalb des Bildschirms',
                   'MenÅ noch aktiv',
                   'MenÅ nicht initialisiert',
                   'Dialogbox enthÑlt keinen Button',
                   'Fehler in Dialogstruktur',
                   'Dialogbox zu gro·',
                   'Fehler in MenÅstruktur');

        errmsge : array [0..11] of string[32] =
                  ('Unknown error',
                   'General error',
                   'Resource-file not found',
                   'Double function call not allowed',
                   'Menu not drawn yet',
                   'Menu out of screen',
                   'Menu still used',
                   'Menu not initialized',
                   'Button missing in dialog box',
                   'Error in dialog structure',
                   'Dialog box too big',
                   'Error in menu structure');

  var fehler : integer;

  begin
    fehler:=num;
    if num<=M_ERROR then
      begin
        if num<_emin then num:=0
          else num:=abs(num)-9;
        write(#7);
        if ccode in [L_USA,L_GB] then void.i:=form_alert('ERROR',errmsge[num],'  #OK  | #Quit ',0)
          else void.i:=form_alert('FEHLER',errmsgd[num],'  #OK  |#Abbruch',0);
        if void.i=1 then
          begin
            hidemouse;
            writeln;
            write('  Runtime Error ',fehler,' :  ');
            if ccode in [L_USA,L_GB] then writeln(errmsge[num]) else writeln(errmsgd[num]);
            writeln;
            halt;
          end;
      end;
  end;


procedure form_seterrl(cc : integer);

  begin
    if cc in [L_USA,L_FRG,L_GB] then ccode:=cc;
  end;


procedure get_hotkey(num : byte);
  var hpos  : byte;
      dummy : string[1];

  begin
    hpos:=pos('#',mwork[num]);
    mhpos[num]:=hpos;
    if hpos=0 then mhkey[num]:=#0
    else if hpos=length(mwork[num]) then
      begin
        mhkey[num]:=#0;
        mhpos[num]:=0;
      end
    else
      begin
        dummy:=mwork[num,succ(hpos)];
        mhkey[num]:=chr(ord(dummy[1]));
        mwork[num]:=left(mwork[num],pred(hpos))+right(mwork[num],length(mwork[num])-hpos);
      end;
  end;


procedure menu_init;
  var q   : integer;

  begin
    asvierror:=0;
    if m_stat=-1 then
      begin
        for q:=0 to _wmax do
          begin
            mwork[q]:=left(work[q],30);
            attrib[q]:=0;
          end;
        q:=1;
        get_hotkey(0);
        m_max:=0;
        mbegin[0]:=0;
        mxpos[0]:=3;
        mxpos[1]:=4+length(mwork[0]);
        repeat
          get_hotkey(q);
          if length(mwork[q])=0 then
            begin
              if length(mwork[succ(q)])<>0 then
                begin
                  inc(q);
                  inc(m_max);
                  mbegin[m_max]:=q;
                  get_hotkey(q);
                  mxpos[succ(m_max)]:=succ(mxpos[m_max])+length(mwork[q]);
                end;
            end;
          inc(q);
        until (q=pred(_wmax)) or ((length(mwork[q])=0) and (length(mwork[succ(q)])=0));
        max_entry:=q;
        for q:=0 to _mmax do mecurr[q]:=1;
        m_num:=0;
        m_stat:=0;
      end
    else
      if m_stat<>2 then
        begin
          asvierror:=E_MUSED;
          form_error(asvierror);
        end;
  end;


procedure menu_draw;
  var q             : integer;
      dummy,owx,owy : byte;

  begin
    asvierror:=0;
    if (m_stat=0) or (m_stat=1) then
      begin
        dummy:=textattr;
        owx:=wherex;
        owy:=wherey;
        hidemouse;
        if m_stat=0 then
          for q:=0 to pred(anzcol) do titleram[q]:=memw[vioseg:(q shl 1)];
        textattr:=schwarzaufweiss;
        gotoxy(1,1);
        write(space(anzcol));
        for q:=0 to m_max do
          begin
            gotoxy(mxpos[q],1);
            write(mwork[mbegin[q]]);
          end;
        textattr:=dummy;
        gotoxy(owx,owy);
        showmouse;
        m_stat:=1;
      end
    else
      if m_stat=-1 then
        begin
          asvierror:=E_NINIT;
          form_error(asvierror);
        end;
  end;


procedure menu_invert(num,was : integer);

  begin
    if was=0 then
      begin
        setattr(mxpos[num]-1,1,weissaufschwarz,succ(mxpos[num+1]-mxpos[num]));
        if mhpos[mbegin[num]]<>0 then setattr(mxpos[num]+mhpos[mbegin[num]]-1,1,hellaufschwarz,1);
      end
    else
      begin
        setattr(mxpos[num]-1,1,schwarzaufweiss,succ(mxpos[num+1]-mxpos[num]));
        if mhpos[mbegin[num]]<>0 then setattr(mxpos[num]+mhpos[mbegin[num]]-1,1,hellaufweiss,1);
      end;
  end;


procedure menu_einvert(num,wo,len,was : integer);

  begin
    inc(wo);
    if was=0 then
      begin
        setattr(wo,mecurr[num]+2,weissaufschwarz,len);
        if mhpos[mbegin[num]+mecurr[num]]<>0 then setattr(wo+mhpos[mbegin[num]+mecurr[num]],mecurr[num]+2,hellaufschwarz,1);
      end
    else
      begin
        setattr(wo,mecurr[num]+2,schwarzaufweiss,len);
        if mhpos[mbegin[num]+mecurr[num]]<>0 then setattr(wo+mhpos[mbegin[num]+mecurr[num]],mecurr[num]+2,hellaufweiss,1);
      end;
  end;


function mo_cmouse : integer;
  var dm_num : integer;
      q,wb   : byte;

  begin
    getmouse(menx,meny,wb);
    menn:=-1;
    if (wb and M_LEFT)=M_LEFT then
      begin
        menk:=true;
        if (meny=1) and (menx>1) and (menx<mxpos[succ(m_max)]) then
          begin
            if menx=2 then dm_num:=0
              else for q:=0 to m_max do if (menx>=mxpos[q]) and (menx<mxpos[succ(q)]) then dm_num:=q;
            if dm_num<>m_num then
              begin
                m_num:=succ(dm_num);
                mo_cmouse:=-3;
              end
            else
              begin
                menn:=mbegin[dm_num];
                mo_cmouse:=0;
              end;
          end;
      end
    else
      begin
        menk:=false;
        mo_cmouse:=0;
      end;
  end;


function menu_open(num : integer) : integer;
  var xc,xl,q,me_anz,vrnum : integer;
      flag,cok,b,h,mmret   : integer;
      oldattr              : byte;
      vofs                 : word;
      taste                : char;

  begin
    flag:=M_NOP;
    xc:=mxpos[num]-2;
    xl:=0;
    q:=mbegin[num]+1;
    me_anz:=0;
    cok:=0;
    while length(mwork[q])>0 do
      begin
        if length(mwork[q])>xl then xl:=length(mwork[q]);
        inc(me_anz);
        if (mwork[q]<>'-') and ((attrib[q] and A_DISABLED)=0) then inc(cok);
        inc(q);
      end;
    if me_anz<(anzline-3) then
      begin
        hidemouse;
        setattr(1,1,schwarzaufweiss,anzcol);
        setattr(mxpos[num]-1,1,weissaufschwarz,succ(mxpos[num+1]-mxpos[num]));
        if me_anz=0 then
          begin
            mp_dis(mbegin[num],1);
            hide;
            showmouse;
            repeat
              mmret:=mo_cmouse;
              if mmret<0 then flag:=mmret
                else if menn>=0 then flag:=menn
                  else if menk and ((meny>1) or (menx<2) or (menx>=mxpos[succ(m_max)])) then flag:=M_CANCEL;
              if keypressed then
                begin
                  taste:=readkey;
                  if keypressed then
                    begin
                      taste:=readkey;
                      case taste of
                        #59: begin
                               hidemouse;
                               byteout[2]:=H_PULL;
                               byteout[3]:=mbegin[num];
                               mp_dis(M_HELP,1);
                               hide;
                               showmouse;
                             end;
                        #75: flag:=-3;
                        #77: flag:=-4;
                        #68: flag:=-5;
                      end;
                    end
                  else
                    case taste of
                      #13: flag:=mbegin[num];
                      #27: flag:=M_CANCEL;
                    end;
                end;
              if kbshift(ALT) then flag:=-5;
            until flag<>M_NOP;
            repeat until not(kbshift(ALT));
            menu_open:=flag;
          end
        else
          begin
            inc(xl,2);
            if farbe then
              begin
                if succ(xc+xl)>pred(anzcol) then xc:=pred(pred(anzcol)-xl);
              end
            else if xc+xl>pred(anzcol) then xc:=pred(anzcol)-xl;
            q:=loff+pred(xc)*2;
            vrnum:=0;
            for h:=0 to me_anz+2 do
              begin
                vofs:=q;
                for b:=0 to xl+2 do
                  begin
                    vioram[vrnum]:=memw[vioseg:vofs];
                    inc(vrnum);
                    inc(vofs,2);
                  end;
                inc(q,loff);
              end;
            oldattr:=textattr;
            textattr:=schwarzaufweiss;
            gotoxy(xc,2);
            write('⁄',strg('ƒ',xl),'ø');
            for q:=1 to me_anz do
              if mwork[mbegin[num]+q]='-' then
                begin
                  gotoxy(xc,q+2);
                  write('√',strg('ƒ',xl),'¥');
                end
              else
                begin
                  gotoxy(xc,q+2);
                  write('≥ ',mwork[mbegin[num]+q],space(xl-length(mwork[mbegin[num]+q])-2),' ≥');
                  if mhpos[mbegin[num]+q]<>0 then setattr(xc+mhpos[mbegin[num]+q]+1,q+2,hellaufweiss,1);
                  if (attrib[mbegin[num]+q] and A_CHECKED)=A_CHECKED then mem[vioseg:((q+1)*loff+xc*2)]:=7;
                  if (attrib[mbegin[num]+q] and A_DISABLED)=A_DISABLED then setattr(xc+1,q+2,disabled,xl);
                end;
            gotoxy(xc,3+me_anz);
            write('¿',strg('ƒ',xl),'Ÿ');
            if farbe then
              begin
                for q:=3 to me_anz+3 do setattr(xc+xl+2,q,shadow,1);
                setattr(succ(xc),4+me_anz,shadow,xl+2);
              end;
            textattr:=oldattr;
            if cok=0 then
              begin
                mp_dis(mbegin[num],1);
                hide;
                showmouse;
                repeat
                  mmret:=mo_cmouse;
                  if mmret<0 then flag:=mmret
                    else if menk and ((meny>1) or (menx<2) or (menx>=mxpos[succ(m_max)])) then flag:=M_CANCEL;
                  if keypressed then
                    begin
                      taste:=readkey;
                      if keypressed then
                        begin
                          taste:=readkey;
                          case taste of
                            #59: begin
                                   hidemouse;
                                   byteout[2]:=H_PULL;
                                   byteout[3]:=mbegin[num];
                                   mp_dis(M_HELP,1);
                                   hide;
                                   showmouse;
                                 end;
                            #75: flag:=-3;
                            #77: flag:=-4;
                            #68: flag:=-5;
                          end;
                        end
                      else if taste in [#13,#27] then flag:=M_CANCEL;
                    end;
                  if kbshift(ALT) then flag:=-5;
                until flag<>M_NOP;
              end
            else
              begin
                if ((mwork[mbegin[num]+mecurr[num]]='-') or
                  ((attrib[mbegin[num]+mecurr[num]] and A_DISABLED)=A_DISABLED)) then mecurr[num]:=1;
                while ((mwork[mbegin[num]+mecurr[num]]='-') or
                  ((attrib[mbegin[num]+mecurr[num]] and A_DISABLED)=A_DISABLED)) do inc(mecurr[num]);
                menu_einvert(num,xc,xl,0);
                mp_dis(mbegin[num]+mecurr[num],1);
                hide;
                showmouse;
                repeat
                  mmret:=mo_cmouse;
                  if mmret<0 then flag:=mmret
                    else if menk then
                      begin
                        if (menx>xc) and (menx<succ(xc+xl)) and (meny>2) and (meny<me_anz+3) then
                          begin
                            if ((mwork[mbegin[num]+meny-2]='-') or
                              ((attrib[mbegin[num]+meny-2] and A_DISABLED)=A_DISABLED)) then flag:=M_CANCEL
                            else
                              begin
                                mecurr[num]:=meny-2;
                                flag:=mbegin[num]+mecurr[num];
                              end;
                          end
                        else if menn<0 then flag:=M_CANCEL;
                      end;
                  if keypressed then
                    begin
                      taste:=readkey;
                      if keypressed then
                        begin
                          taste:=readkey;
                          case taste of
                            #59: begin
                                   hidemouse;
                                   byteout[2]:=H_PULL;
                                   byteout[3]:=mbegin[num]+mecurr[num];
                                   mp_dis(M_HELP,1);
                                   hide;
                                   showmouse;
                                 end;
                            #75: flag:=-3;
                            #77: flag:=-4;
                            #68: flag:=-5;
                            #72: begin
                                   hidemouse;
                                   menu_einvert(num,xc,xl,1);
                                   dec(mecurr[num]);
                                   if mecurr[num]<1 then mecurr[num]:=me_anz;
                                   while ((mwork[mbegin[num]+mecurr[num]]='-') or
                                     ((attrib[mbegin[num]+mecurr[num]] and A_DISABLED)=A_DISABLED)) do dec(mecurr[num]);
                                   if mecurr[num]<1 then mecurr[num]:=me_anz;
                                   while ((mwork[mbegin[num]+mecurr[num]]='-') or
                                     ((attrib[mbegin[num]+mecurr[num]] and A_DISABLED)=A_DISABLED)) do dec(mecurr[num]);
                                   menu_einvert(num,xc,xl,0);
                                   mp_dis(mbegin[num]+mecurr[num],1);
                                   hide;
                                   showmouse;
                                 end;
                            #80: begin
                                   hidemouse;
                                   menu_einvert(num,xc,xl,1);
                                   inc(mecurr[num]);
                                   if mecurr[num]>me_anz then mecurr[num]:=1;
                                   while ((mwork[mbegin[num]+mecurr[num]]='-') or
                                     ((attrib[mbegin[num]+mecurr[num]] and A_DISABLED)=A_DISABLED)) do inc(mecurr[num]);
                                   if mecurr[num]>me_anz then mecurr[num]:=1;
                                   while ((mwork[mbegin[num]+mecurr[num]]='-') or
                                     ((attrib[mbegin[num]+mecurr[num]] and A_DISABLED)=A_DISABLED)) do inc(mecurr[num]);
                                   menu_einvert(num,xc,xl,0);
                                   mp_dis(mbegin[num]+mecurr[num],1);
                                   hide;
                                   showmouse;
                                 end;
                          end;
                        end
                      else
                        case taste of
                          #13: flag:=mbegin[num]+mecurr[num];
                          #27: flag:=M_CANCEL;
                        else
                          for q:=1 to me_anz do
                            if mhpos[mbegin[num]+q]<>0 then
                              if upcase(taste)=upcase(mhkey[mbegin[num]+q]) then
                                if (attrib[mbegin[num]+q] and A_DISABLED)=0 then
                                  begin
                                    mecurr[num]:=q;
                                    flag:=mbegin[num]+q;
                                  end;
                        end;
                    end;
                  if kbshift(ALT) then flag:=-5;
                until flag<>M_NOP;
              end;
            hidemouse;
            q:=loff+pred(xc)*2;
            vrnum:=0;
            for h:=0 to me_anz+2 do
              begin
                vofs:=q;
                for b:=0 to xl+2 do
                  begin
                    memw[vioseg:vofs]:=vioram[vrnum];
                    inc(vrnum);
                    inc(vofs,2);
                  end;
                inc(q,loff);
              end;
            showmouse;
            repeat until not(kbshift(ALT));
            menu_open:=flag;
          end;
      end
    else menu_open:=E_MOUTSCR;
  end;


function menu_odo : integer;
  var flag : integer;

  begin
    repeat
      flag:=menu_open(m_num);
      case flag of
        -3: begin
              dec(m_num);
              if m_num<0 then m_num:=m_max;
            end;
        -4: begin
              inc(m_num);
              if m_num>m_max then m_num:=0;
            end;
      end;
    until (flag=-5) or (flag=M_CANCEL) or (flag=E_MOUTSCR) or (flag>=M_OK);
    hidemouse;
    mp_dis(M_EXIT,1);
    hide;
    showmouse;
    menu_odo:=flag;
  end;


function menu_do : integer;
  label altjmp,f10jmp;

  var taste      : char;
      q,flag,ret : integer;
      wx,wy,wb   : byte;

  begin
    altjmp:
    flag:=M_NOP;
    taste:=#0;
    hidemouse;
    for q:=0 to m_max do menu_invert(q,1);
    menu_invert(m_num,0);
    showmouse;
    while keypressed do void.c:=readkey;
    repeat until not(kbshift(ALT));
    if (byteout[0]=KEY_SPEC) and (byteout[1]=K_F10) and (byteout[4]=255) then
      begin
        taste:=#80;
        byteout[4]:=0;
        goto f10jmp;
      end;
    repeat
      getmouse(wx,wy,wb);
      if (wb and M_LEFT)=M_LEFT then
        if (wy=1) and (wx>1) and (wx<mxpos[succ(m_max)]) then
          begin
            if wx=2 then m_num:=0
              else for q:=0 to m_max do if (wx>=mxpos[q]) and (wx<mxpos[succ(q)]) then m_num:=q;
            if length(mwork[succ(mbegin[m_num])])=0 then flag:=mbegin[m_num]
              else flag:=menu_odo;
          end;
      if keypressed then
        begin
          taste:=readkey;
          if keypressed then
            begin
              taste:=readkey;
              f10jmp:
              case taste of
                #59: begin
                       hidemouse;
                       byteout[2]:=H_PULL;
                       byteout[3]:=mbegin[m_num];
                       mp_dis(M_HELP,1);
                       hide;
                       showmouse;
                     end;
                #75: begin
                       hidemouse;
                       menu_invert(m_num,1);
                       dec(m_num);
                       if m_num<0 then m_num:=m_max;
                       menu_invert(m_num,0);
                       showmouse;
                     end;
                #77: begin
                       hidemouse;
                       menu_invert(m_num,1);
                       inc(m_num);
                       if m_num>m_max then m_num:=0;
                       menu_invert(m_num,0);
                       showmouse;
                     end;
                #68: flag:=M_CANCEL;
                #80,#72: if length(mwork[succ(mbegin[m_num])])>0 then flag:=menu_odo;
              end;
              taste:=#0;
            end
          else
            if taste=#13 then
              begin
                if length(mwork[succ(mbegin[m_num])])=0 then flag:=mbegin[m_num]
                  else flag:=menu_odo;
              end
            else
              begin
                for q:=0 to m_max do
                  if mhpos[mbegin[q]]<>0 then
                    if upcase(taste)=upcase(mhkey[mbegin[q]]) then
                      begin
                        m_num:=q;
                        if length(mwork[succ(mbegin[m_num])])=0 then flag:=mbegin[m_num]
                          else flag:=menu_odo;
                      end;
              end;
        end;
      if (kbshift(ALT)) or (taste=#27) then flag:=M_CANCEL;
    until flag<>M_NOP;
    hidemouse;
    setattr(1,1,schwarzaufweiss,anzcol);
    showmouse;
    repeat until not(kbshift(ALT));
    if flag>=M_OK then repeat until mouseb=0;
    while keypressed do void.c:=readkey;
    if flag=-5 then goto altjmp;
    menu_do:=flag;
  end;


function menu_watch : integer;
  var dummy,owx,owy,q,wx,wy,wb : byte;
      mflag                    : boolean;

  begin
    if m_stat=1 then
      begin
        mflag:=false;
        byteout[0]:=KEY_NONE;
        byteout[1]:=0;
        if kbshift(ALT) then mflag:=true;
        if keypressed then
          begin
            byteout[1]:=ord(readkey);
            if keypressed then
              begin
                byteout[0]:=KEY_SPEC;
                byteout[1]:=ord(readkey);
                if byteout[1]=K_F10 then mflag:=true;
              end
            else byteout[0]:=KEY_NORM;
          end;
        getmouse(wx,wy,wb);
        if (wb and M_LEFT)=M_LEFT then
          if (wy=1) and (wx>1) and (wx<mxpos[succ(m_max)]) then
            begin
              byteout[0]:=KEY_SPEC;
              byteout[1]:=K_F10;
              if wx=2 then m_num:=0
              else
                for q:=0 to m_max do
                  if (wx>=mxpos[q]) and (wx<mxpos[succ(q)]) then m_num:=q;
              mflag:=true;
            end;
        if mflag then
          begin
            dummy:=textattr;
            owx:=wherex;
            owy:=wherey;
            byteout[4]:=255;
            menu_watch:=menu_do;
            textattr:=dummy;
            gotoxy(owx,owy);
            byteout[0]:=KEY_NONE;
            byteout[1]:=0;
          end
        else menu_watch:=M_NOP;
      end
    else if m_stat=2 then menu_watch:=E_NREENT
    else menu_watch:=M_ERROR;
  end;


procedure menu_kill;
  var q : integer;

  begin
    asvierror:=0;
    if m_stat=1 then
      begin
        hidemouse;
        for q:=0 to pred(anzcol) do memw[vioseg:(q shl 1)]:=titleram[q];
        m_stat:=-1;
        showmouse;
      end
    else
      if m_stat<>2 then
        begin
          asvierror:=E_NDRAW;
          form_error(asvierror);
        end;
  end;


procedure menu_add(wrk : string);

  begin
    if (max_entry<pred(_wmax)) and (length(wrk)>0) then
      begin
        mwork[max_entry]:=left(wrk,30);
        get_hotkey(max_entry);
        inc(max_entry);
        mwork[succ(max_entry)]:='';
      end
    else
      begin
        asvierror:=E_MSTRUC;
        form_error(asvierror);
      end;
  end;


procedure menu_delete(num : byte);
  var q : byte;

  begin
    if (num<=_wmax) and (num>mbegin[m_max]) and (length(mwork[num])>0) then
      begin
        for q:=num to max_entry do
          begin
            mwork[q]:=mwork[succ(q)];
            mhpos[q]:=mhpos[succ(q)];
            mhkey[q]:=mhkey[succ(q)];
            attrib[q]:=attrib[succ(q)];
          end;
        dec(max_entry);
        mecurr[m_max]:=1;
      end
    else
      begin
        asvierror:=E_MSTRUC;
        form_error(asvierror);
      end;
  end;


procedure menu_rename(num : byte; wrk : string);
  var q,flag : byte;

  begin
    asvierror:=0;
    if m_stat=2 then asvierror:=E_NREENT
    else
      begin
        if num>_wmax then
          begin
            asvierror:=E_MSTRUC;
            form_error(asvierror);
          end
        else
          begin
            if (length(mwork[num])=0) or (length(wrk)=0) then
              begin
                asvierror:=E_MSTRUC;
                form_error(asvierror);
              end
            else
              begin
                flag:=0;
                for q:=0 to m_max do if num=mbegin[q] then flag:=1;
                if flag=1 then
                  begin
                    asvierror:=E_MSTRUC;
                    form_error(asvierror);
                  end
                else
                  begin
                    mwork[num]:=left(wrk,30);
                    get_hotkey(num);
                  end;
              end;
          end;
      end;
  end;


procedure menu_pinvert(num,wox,woy,len,was,woh : byte);

  begin
    if was=0 then
      begin
        setattr(wox,woy,weissaufschwarz,len);
        if woh>0 then setattr(wox+woh,woy,hellaufschwarz,1);
      end
    else
      begin
        setattr(wox,woy,schwarzaufweiss,len);
        if woh>0 then setattr(wox+woh,woy,hellaufweiss,1);
      end;
  end;


function menu_pop(wo : byte) : integer;
  label all_dis;

  var flag,plen,plmax,q,vrnum,vofs,def,oms : integer;
      taste                                : char;
      mpnum,owx,owy,xp,yp,dtxattr,wx,wy,wb : byte;
      ppos,brt,hhe,cflag,h,b,dflag,dpos    : byte;
      phkpos,pmattr,spstr                  : string;
      dummy                                : string[1];

  begin
    if m_stat<>2 then
      begin
        owx:=wherex;
        owy:=wherey;
        dtxattr:=textattr;
        oms:=m_stat;
        if length(work[wo])<4 then
          begin
            menu_pop:=E_MSTRUC;
            exit;
          end;
        xp:=ord(work[wo,1]);
        yp:=ord(work[wo,2]);
        def:=ord(work[wo,3]);
        q:=succ(wo);
        flag:=0;
        plmax:=0;
        pmattr:='';
        phkpos:='';
        dhkstr:='';
        repeat
          inc(q);
          dwork[q]:=work[q];
          plen:=length(dwork[q]);
          if plen>1 then
            begin
              dec(plen);
              pmattr:=pmattr+left(dwork[q],1);
              dwork[q]:=right(dwork[q],plen);
              ppos:=pos('#',dwork[q]);
              if (ppos>0) and (ppos<>plen) then
                begin
                  phkpos:=phkpos+chr(ppos);
                  dummy:=dwork[q,succ(ppos)];
                  dhkstr:=dhkstr+upcase(dummy[1]);
                  dwork[q]:=left(dwork[q],pred(ppos))+right(dwork[q],length(dwork[q])-ppos);
                end
              else
                begin
                  dhkstr:=dhkstr+#0;
                  phkpos:=phkpos+#0;
                end;
              if length(dwork[q])>plmax then plmax:=length(dwork[q]);
            end
          else
            begin
              if dwork[q]='-' then
                begin
                  pmattr:=pmattr+chr(A_DISABLED);
                  dhkstr:=dhkstr+#0;
                  phkpos:=phkpos+#0;
                end
              else if plen=1 then flag:=1;
            end;
        until (q=pred(_wmax)) or ((length(work[q])=0) and (length(work[succ(q)])=0));
        if (flag<>0) or (plmax=0) then
          begin
            menu_pop:=E_MSTRUC;
            exit;
          end;
        brt:=plmax+4;
        hhe:=succ(pred(q)-wo);
        if xp=255 then xp:=succ((anzcol-brt) shr 1);
        if yp=255 then yp:=succ((anzline shr 1)-(hhe shr 1));
        if xp<1 then xp:=1;
        if yp<1 then yp:=1;
        if succ(brt)*succ(hhe)>_viormax then
          begin
            menu_pop:=E_DTBIG;
            exit;
          end;
        if farbe then cflag:=0 else cflag:=1;
        if (xp+brt-cflag>anzcol) or (yp+hhe>anzline) or (hhe<3) then
          begin
            menu_pop:=E_MOUTSCR;
            exit;
          end;
        hidemouse;
        q:=pred(yp)*loff+pred(xp)*2;
        vrnum:=0;
        for h:=0 to hhe do
          begin
            vofs:=q;
            for b:=0 to brt do
              begin
                vioram[vrnum]:=memw[vioseg:vofs];
                inc(vrnum);
                inc(vofs,2);
              end;
            inc(q,loff);
          end;
        textattr:=schwarzaufweiss;
        gotoxy(xp,yp);
        write('⁄',strg('ƒ',brt-2),'ø');
        spstr:='≥'+space(brt-2)+'≥';
        dflag:=0;
        for q:=succ(yp) to yp+hhe-2 do
          begin
            gotoxy(xp,q);
            if dwork[succ(wo+q-yp)]='-' then write('√',strg('ƒ',brt-2),'¥')
            else
              begin
                write(spstr);
                gotoxy(xp+2,q);
                write(dwork[succ(wo+q-yp)]);
                if (ord(pmattr[q-yp]) and A_CHECKED)=A_CHECKED then mem[vioseg:(pred(q)*loff+xp*2)]:=7;
                if (ord(pmattr[q-yp]) and A_DISABLED)=A_DISABLED then setattr(succ(xp),q,disabled,brt-2)
                else
                  begin
                    if ord(phkpos[q-yp])>0 then setattr(succ(xp+ord(phkpos[q-yp])),q,hellaufweiss,1);
                    dflag:=1;
                  end;
              end;
          end;
        gotoxy(xp,pred(yp+hhe));
        write('¿',strg('ƒ',brt-2),'Ÿ');
        if farbe then
          begin
            for q:=succ(yp) to yp+hhe do setattr(xp+brt,q,shadow,1);
            setattr(succ(xp),yp+hhe,shadow,brt);
          end;
        if length(work[succ(wo)])>0 then
          begin
            gotoxy(pred(xp+(brt shr 1)-(length(work[succ(wo)]) shr 1)),yp);
            write(' ',work[succ(wo)],' ');
          end;
        if dflag=0 then
          begin
            hide;
            showmouse;
            repeat
              if keypressed then
                begin
                  taste:=readkey;
                  if keypressed then
                    if readkey=#59 then
                      begin
                        hidemouse;
                        byteout[2]:=H_POP;
                        byteout[3]:=M_DIS;
                        mp_dis(M_HELP,oms);
                        hide;
                        showmouse;
                      end;
                end;
              getmouse(wx,wy,wb);
              if (wb and M_LEFT)=M_LEFT then
                if ((wx<=xp) or (wx>=pred(xp+brt)) or (wy<=yp) or (wy>=pred(yp+hhe))) then taste:=#27;
            until taste in [#13,#27];
            flag:=M_CANCEL;
            hidemouse;
            goto all_dis;
          end;
        if def>(hhe-3) then def:=0;
        if (dwork[wo+def+2]='-') or (ord(pmattr[succ(def)])=A_DISABLED) then def:=0;
        while (dwork[wo+def+2]='-') or (ord(pmattr[succ(def)])=A_DISABLED) do inc(def);
        menu_pinvert(wo+def+2,succ(xp),succ(yp+def),brt-2,0,ord(phkpos[succ(def)]));
        textattr:=dtxattr;
        flag:=M_NOP;
        mp_dis(P_OFFS+def,oms);
        hide;
        showmouse;
        repeat
          getmouse(wx,wy,wb);
          if (wb and M_LEFT)=M_LEFT then
            begin
              if ((wx<=xp) or (wx>=pred(xp+brt)) or (wy<=yp) or (wy>=pred(yp+hhe))) then flag:=M_CANCEL
              else
                begin
                  if (dwork[wo+pred(wy-yp)+2]='-') or (ord(pmattr[wy-yp])=A_DISABLED) then flag:=M_CANCEL
                    else flag:=pred(wy-yp);
                end;
            end;
          if keypressed then
            begin
              taste:=readkey;
              if keypressed then
                begin
                  taste:=readkey;
                  case taste of
                    #59: begin
                           hidemouse;
                           byteout[2]:=H_POP;
                           byteout[3]:=def;
                           mp_dis(M_HELP,oms);
                           hide;
                           showmouse;
                         end;
                    #72: begin
                           hidemouse;
                           menu_pinvert(wo+def+2,succ(xp),succ(yp+def),brt-2,1,ord(phkpos[succ(def)]));
                           dec(def);
                           if def<0 then def:=hhe-3;
                           while (((dwork[wo+def+2]='-') or (ord(pmattr[succ(def)])=A_DISABLED)) and (def>0)) do dec(def);
                           if (def=0) and ((dwork[wo+2]='-') or (ord(pmattr[1])=A_DISABLED)) then def:=hhe-3;
                           while (dwork[wo+def+2]='-') or (ord(pmattr[succ(def)])=A_DISABLED) do dec(def);
                           menu_pinvert(wo+def+2,succ(xp),succ(yp+def),brt-2,0,ord(phkpos[succ(def)]));
                           mp_dis(P_OFFS+def,oms);
                           hide;
                           showmouse;
                         end;
                    #80: begin
                           hidemouse;
                           menu_pinvert(wo+def+2,succ(xp),succ(yp+def),brt-2,1,ord(phkpos[succ(def)]));
                           inc(def);
                           if def>hhe-3 then def:=0;
                           while (((dwork[wo+def+2]='-') or (ord(pmattr[succ(def)])=A_DISABLED)) and (def<hhe-3)) do inc(def);
                           if (def=hhe-3) and ((dwork[pred(wo+hhe)]='-') or (ord(pmattr[hhe-2])=A_DISABLED)) then def:=0;
                           while (dwork[wo+def+2]='-') or (ord(pmattr[succ(def)])=A_DISABLED) do inc(def);
                           menu_pinvert(wo+def+2,succ(xp),succ(yp+def),brt-2,0,ord(phkpos[succ(def)]));
                           mp_dis(P_OFFS+def,oms);
                           hide;
                           showmouse;
                         end;
                  end;
                end
              else
                case taste of
                  #13: flag:=def;
                  #27: flag:=M_CANCEL;
                else
                  begin
                    dpos:=pos(upcase(taste),dhkstr);
                    if (dpos>0) and (dpos<=(hhe-2)) then
                      if ord(pmattr[dpos])<>A_DISABLED then flag:=pred(dpos);
                  end;
                end;
            end;
        until flag<>M_NOP;
        hidemouse;
        mp_dis(M_EXIT,oms);
        all_dis:
        q:=pred(yp)*loff+pred(xp)*2;
        vrnum:=0;
        for h:=0 to hhe do
          begin
            vofs:=q;
            for b:=0 to brt do
              begin
                memw[vioseg:vofs]:=vioram[vrnum];
                inc(vrnum);
                inc(vofs,2);
              end;
            inc(q,loff);
          end;
        textattr:=dtxattr;
        gotoxy(owx,owy);
        repeat until mouseb=0;
        showmouse;
        menu_pop:=flag;
      end
    else menu_pop:=E_NREENT;
  end;


procedure init;

  begin
    regs.ah:=15;
    intr($10,regs);
    if regs.al=7 then vioseg:=$b000 else vioseg:=$b800;
    farbe:=not((regs.al=0) or (regs.al=2) or (regs.al=7));
    anzcol:=regs.ah;
    anzline:=25;
    loff:=anzcol shl 1;
    regs.ah:=5;
    regs.al:=0;
    intr($10,regs);
    mausda:=false;
    manzbut:=0;
    if meml[$0000:$00cc]>0 then
      begin
        regs.ax:=0;
        regs.bx:=0;
        intr($33,regs);
        if (regs.ax=$ffff) and (regs.bx in [2,3]) then
          begin
            mausda:=true;
            manzbut:=regs.bx;
            regs.ax:=29;
            regs.bx:=0;
            intr($33,regs);
          end;
      end;
    mausvis:=false;
  end;


procedure menu_setc(was,saw,has,haw,dis,shdw : byte);

  begin
    if m_stat<>2 then
      begin
        weissaufschwarz:=was;
        schwarzaufweiss:=saw;
        hellaufschwarz:=has;
        hellaufweiss:=haw;
        disabled:=dis;
        shadow:=shdw;
      end;
  end;


procedure menu_resetc;

  begin
    if farbe then menu_setc(attr(lightgray,black),attr(black,lightgray),attr(green,black),
      attr(red,lightgray),attr(brown,lightgray),attr(black,black))
    else menu_setc(7,112,15,127,127,0);
  end;


procedure scrforce(anzx,anzy : byte; avcol : boolean);

  begin
    farbe:=avcol;
    anzcol:=anzx;
    anzline:=anzy;
    loff:=anzcol shl 1;
    menu_resetc;
  end;


procedure do_nothing(wnum : byte); begin end;


procedure menu_setp(procv : pvtype);

  begin
    m_pcall:=procv;
  end;


procedure menu_resetp;

  begin
    menu_setp(do_nothing);
  end;


procedure menu_attr(num,attrs : byte);

  begin
    if (m_stat<>2) and (num<=_wmax) then
      case attrs of
        A_NCHECKED: attrib[num]:=attrib[num] and A_DISABLED;
        A_CHECKED:  attrib[num]:=attrib[num] or A_CHECKED;
        A_ENABLED:  attrib[num]:=attrib[num] and A_CHECKED;
        A_DISABLED: attrib[num]:=attrib[num] or A_DISABLED;
      end;
  end;


function menu_stat(num : byte) : byte;

  begin
    if num<=_wmax then menu_stat:=attrib[num]
      else menu_stat:=0;
  end;


function menu_text(num : byte) : string;

  begin
    if num<=_wmax then menu_text:=mwork[num]
      else menu_text:='';
  end;


procedure menu_inquire;

  begin
    byteout[0]:=weissaufschwarz;
    byteout[1]:=schwarzaufweiss;
    byteout[2]:=hellaufschwarz;
    byteout[3]:=hellaufweiss;
    byteout[4]:=disabled;
    byteout[5]:=shadow;
    byteout[6]:=0;
    byteout[7]:=0;
    byteout[8]:=0;
    byteout[9]:=0;
    byteout[10]:=manzbut;
    byteout[11]:=PC;
    byteout[12]:=28;
    byteout[13]:=10;
    byteout[14]:=90;
    intout[0]:=ccode;
    intout[1]:=L_USA+L_FRG+L_GB;
    intout[2]:=_emin;
    intr($12,regs);
    intout[3]:=regs.ax;
    intout[4]:=0;
    intout[5]:=_viormax;
    intout[6]:=0;
    intout[7]:=pred(max_entry);
    intout[8]:=0;
    intout[9]:=0;
  end;


function _scrx : byte;

  begin
    _scrx:=anzcol;
  end;


function _scry : byte;

  begin
    _scry:=anzline;
  end;


function color : boolean;

  begin
    color:=farbe;
  end;


function mouse : boolean;

  begin
    mouse:=mausda;
  end;


function mvisible : boolean;

  begin
    mvisible:=mausvis;
  end;


procedure mouse_sets(sx,sy : integer);

  begin
    if mausda then
      begin
        if sx<1 then sx:=1;
        if sy<1 then sy:=1;
        regs.ax:=15;
        regs.cx:=sx;
        regs.dx:=sy;
        intr($33,regs);
      end;
  end;


procedure mouse_resets;

  begin
    mouse_sets(8,16);
  end;


procedure mouse_sett(dst : integer);

  begin
    if mausda then
      begin
        if dst<0 then dst:=0;
        regs.ax:=19;
        regs.dx:=dst;
        intr($33,regs);
      end;
  end;


procedure mouse_resett;

  begin
    mouse_sett(64);
  end;


function asverror : integer;

  begin
    asverror:=asvierror;
    asvierror:=0;
  end;


procedure asvexit;

  begin
    exitproc:=exitsave;
    if mausda then
      begin
        hidemouse;
        regs.ax:=0;
        intr($33,regs);
      end;
  end;


begin
  init;
  asvierror:=0;
  form_seterrl(L_FRG);
  menu_resetc;
  menu_resetp;
  menu_inquire;
  m_stat:=-1;
  exitsave:=exitproc;
  exitproc:=@asvexit;
  showmouse;
  defmouse($ffff,$7700);
  mouse_resets;
  mouse_resett;
end.