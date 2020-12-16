{ ASV 1.5 PRCS vom 18.12.1991 }

function upchar(ch : char) : char;
  var dummy : byte;

  begin
    dummy:=ord(ch);
    if (dummy>96) and (dummy<123) then dec(dummy,32)
    else
      case dummy of
        132: dummy:=142;
        148: dummy:=153;
        129: dummy:=154;
      end;
    upchar:=chr(dummy);
  end;


procedure hide;

  begin
    gotoxy(anzcol,anzline);
  end;


procedure hidecursor;

  begin
    regs.ax:=$300;
    intr($10,regs);
    regs.cx:=regs.cx or $2000;
    regs.ax:=$100;
    intr($10,regs);
  end;


procedure showcursor;

  begin
    regs.ax:=$300;
    intr($10,regs);
    regs.cx:=regs.cx and $dfff;
    regs.ax:=$100;
    intr($10,regs);
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


procedure cutl(var teil : string);

  begin
    if length(teil)>0 then teil:=right(teil,pred(length(teil))) else teil:='';
  end;


procedure cutr(var teil : string);

  begin
    if length(teil)>0 then teil:=left(teil,pred(length(teil))) else teil:='';
  end;


function trimf(teil : string) : string;

  begin
    while left(teil,1)=' ' do cutl(teil);
    while right(teil,1)=' ' do cutr(teil);
    trimf:=teil;
  end;


procedure trim(var teil : string);

  begin
    teil:=trimf(teil);
  end;


function upper(teil : string) : string;
  var q : byte;

  begin
    if length(teil)>0 then
      for q:=1 to length(teil) do teil[q]:=upchar(teil[q]);
    upper:=teil;
  end;


function lower(teil : string) : string;
  var q,dummy : byte;

  begin
    if length(teil)>0 then
      for q:=1 to length(teil) do
        begin
          dummy:=ord(teil[q]);
          if (dummy>64) and (dummy<91) then inc(dummy,32)
          else
            case dummy of
              142: dummy:=132;
              153: dummy:=148;
              154: dummy:=129;
            end;
          teil[q]:=chr(dummy);
        end;
    lower:=teil;
  end;


function sgn(wert : longint) : shortint;

  begin
    if wert<0 then sgn:=-1
    else if wert>0 then sgn:=1
    else sgn:=0;
  end;


function ftoa(zahl : real) : string;

  begin
    str(zahl:0:11,sdummy);
    while right(sdummy,1)='0' do cutr(sdummy);
    if right(sdummy,1)='.' then sdummy:=sdummy+'0';
    ftoa:=sdummy;
  end;


function atof(zeichen : string) : real;
  var sflag       : boolean;
      zahl,teiler : real;

  begin
    zahl:=0;
    trim(zeichen);
    if zeichen[1]='-' then sflag:=true else sflag:=false;
    if (zeichen[1]='-') or (zeichen[1]='+') then cutl(zeichen);
    while (length(zeichen)>0) and (zeichen[1]>='0') and (zeichen[1]<='9') and (zahl<=99999999) do
      begin
        zahl:=zahl*10+ord(zeichen[1])-48;
        cutl(zeichen);
      end;
    cutl(zeichen);
    teiler:=10;
    while (length(zeichen)>0) and (zeichen[1]>='0') and (zeichen[1]<='9') and (teiler<1E12) do
      begin
        zahl:=zahl+(ord(zeichen[1])-48)/teiler;
        cutl(zeichen);
        teiler:=teiler*10;
      end;
    if sflag then atof:=-zahl else atof:=zahl;
  end;


function itoa(zahl : longint) : string;

  begin
    str(zahl,sdummy);
    itoa:=sdummy;
  end;


function atoi(zeichen : string) : longint;
  var zahl : real;

  begin
    zahl:=atof(zeichen);
    while zahl>maxlongint do zahl:=zahl/10;
    atoi:=trunc(zahl);
  end;


procedure hardcopy;

  begin
    intr($5,regs);
  end;


procedure printxy(x,y : byte; txt : string);
  var vmem      : word;
      sofs,sseg : word;
      q         : byte;

  begin
    sseg:=seg(txt);
    sofs:=succ(ofs(txt));
    vofs:=(anzcol shl 1)*pred(y)+(pred(x) shl 1);
    vmem:=textattr shl 8;
    for q:=1 to length(txt) do
      begin
        memw[vioseg:vofs]:=vmem+mem[sseg:sofs];
        inc(vofs,2);
        inc(sofs);
      end;
  end;


function getvmemp(var p : pointer) : boolean;

  begin
    if m_stat<>2 then
      begin
        p:=@vioram[0];
        getvmemp:=true;
      end
    else
      begin
        p:=nil;
        getvmemp:=false;
      end;
  end;


function prn_stat(bits : byte) : boolean;

  begin
    regs.ah:=2;
    regs.dx:=0;
    intr($17,regs);
    if (regs.ah and bits)=bits then prn_stat:=true
      else prn_stat:=false;
  end;


function kbshift(bits : byte) : boolean;

  begin
    regs.ah:=2;
    intr($16,regs);
    if (regs.al and bits)=bits then kbshift:=true
      else kbshift:=false;
  end;


procedure kbset(bits : byte);

  begin
    if (bits=SCRLLOCK) or (bits=NUMLOCK) or (bits=CAPSLOCK) then
      begin
        mem[$0040:$0017]:=mem[$0040:$0017] or bits;
        regs.ah:=2;
        intr($16,regs);
      end;
  end;


procedure kbclr(bits : byte);

  begin
    if (bits=SCRLLOCK) or (bits=NUMLOCK) or (bits=CAPSLOCK) then
      begin
        mem[$0040:$0017]:=mem[$0040:$0017] and (255-bits);
        regs.ah:=2;
        intr($16,regs);
      end;
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
    if (m_stat<>2) and mausda then
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


procedure clsp(att,zch : byte);
  var catt,start : word;

  begin
    hidemouse;
    catt:=(att shl 8)+zch;
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
    for wq:=start to pred(anzcol*anzline) do
      begin
        memw[vioseg:vofs]:=catt;
        inc(vofs,2);
      end;
    showmouse;
  end;


procedure cls(att : byte);

  begin
    clsp(att,C_SPACE);
  end;


procedure setattr(tx,ty,art,len : byte);

  begin
    vofs:=succ(pred(ty)*loff+pred(tx)*2);
    for wq:=1 to len do
      begin
        mem[vioseg:vofs]:=art;
        inc(vofs,2);
      end;
  end;


procedure get_vioram(xp,yp,brt,hhe,cflag : byte);
  var vrnum  : integer;
      h,b    : byte;

  begin
    wq:=pred(yp)*loff+pred(xp)*2;
    vrnum:=0;
    for h:=0 to hhe-cflag do
      begin
        vofs:=wq;
        for b:=0 to brt-cflag do
          begin
            vioram[vrnum]:=memw[vioseg:vofs];
            inc(vrnum);
            inc(vofs,2);
          end;
        inc(wq,loff);
      end;
  end;


procedure put_vioram(xp,yp,brt,hhe,cflag : byte);
  var vrnum  : integer;
      h,b    : byte;

  begin
    wq:=pred(yp)*loff+pred(xp)*2;
    vrnum:=0;
    for h:=0 to hhe-cflag do
      begin
        vofs:=wq;
        for b:=0 to brt-cflag do
          begin
            memw[vioseg:vofs]:=vioram[vrnum];
            inc(vrnum);
            inc(vofs,2);
          end;
        inc(wq,loff);
      end;
  end;


procedure dobj_nothing(wnum,com,dat1,dat2 : byte); begin end;


function get_fobjmax : byte;
  var q,anz : byte;

  begin
    anz:=0;
    for q:=0 to _dobjmax do if @dobjmem[q]=@dobj_nothing then inc(anz);
    get_fobjmax:=anz;
  end;


function form_objins(nobj : dobjtype) : integer;
  var q : byte;

  begin
    if get_fobjmax>0 then
      begin
        q:=0;
        while @dobjmem[q]<>@dobj_nothing do inc(q);
        dobjmem[q]:=nobj;
        form_objins:=q;
      end
    else form_objins:=F_ERROR;
  end;


function form_objdel(objn : byte) : integer;

  begin
    if (objn>_dobjmax) or (objn=do_anum) or (objn=do_rnum) or (objn=do_pnum) then form_objdel:=F_ERROR
    else
      begin
        dobjmem[objn]:=dobj_nothing;
        form_objdel:=F_OK;
      end;
  end;


function form_objreplace(objn : byte; nobj : dobjtype) : integer;

  begin
    if objn>_dobjmax then form_objreplace:=F_ERROR
    else
      begin
        dobjmem[objn]:=nobj;
        form_objreplace:=objn;
      end;
  end;


procedure form_objcall(objn,wnum,com,dat1,dat2 : byte);

  begin
    if (objn<=_dobjmax) and (wnum<=_wmax) then
      dobjmem[objn](wnum,com,dat1,dat2);
  end;


function dtext(dx,dy : byte; dt : string) : string;

  begin
    dtext:=chr(dx)+chr(dy)+dt;
  end;


function dframe(dx,dy,db,dh : byte; dt : string) : string;

  begin
    dframe:=#255#0+chr(dx)+chr(dy)+chr(db)+chr(dh)+dt;
  end;


function dpush(dx,dy,ds : byte; title : string) : string;

  begin
    if (ds>B_NXOFF) and (ds<B_DISABLED) then ds:=B_ON;
    dpush:=#255+chr(do_pnum)+chr(dx)+chr(dy)+chr(ds)+title;
  end;


function dradio(dx,dy,dn,ds : byte; title : string) : string;

  begin
    if (ds>B_ON) and (ds<B_DISABLED) then ds:=B_ON;
    dradio:=#255+chr(do_rnum)+chr(dx)+chr(dy)+chr(dn)+chr(ds)+title;
  end;


function dalpha(dx,dy,db,dmb,ds : byte; title,cont : string) : string;

  begin
    if ds<>B_DISABLED then ds:=B_ENABLED;
    title:=left(title,76);
    if dmb+length(title)>80 then dmb:=80-length(title);
    dalpha:=#255+chr(do_anum)+chr(dx)+chr(dy)+chr(db)+chr(dmb)+chr(ds)+chr(length(title))+title+left(cont,dmb);
  end;


procedure form_setpb(wnum,ds : byte);

  begin
    if wnum<=_wmax then
      if (work[wnum,1]=#255) and (work[wnum,2]=chr(do_pnum)) and (length(work[wnum])>4) then
        begin
          if (ds>B_NXOFF) and (ds<B_DISABLED) then ds:=B_ON;
          work[wnum,5]:=chr(ds);
        end;
  end;


function form_getpb(wnum : byte) : byte;

  begin
    if wnum<=_wmax then
      begin
        if (work[wnum,1]=#255) and (work[wnum,2]=chr(do_pnum)) then form_getpb:=ord(work[wnum,5])
          else form_getpb:=0;
      end
    else form_getpb:=0;
  end;


procedure form_setrb(wnum,ds : byte);

  begin
    if wnum<=_wmax then
      if (work[wnum,1]=#255) and (work[wnum,2]=chr(do_rnum)) and (length(work[wnum])>5) then
        begin
          if (ds>B_ON) and (ds<B_DISABLED) then ds:=B_ON;
          work[wnum,6]:=chr(ds);
        end;
  end;


function form_getrb(wnum : byte) : byte;

  begin
    if wnum<=_wmax then
      begin
        if (work[wnum,1]=#255) and (work[wnum,2]=chr(do_rnum)) then form_getrb:=ord(work[wnum,6])
          else form_getrb:=0;
      end
    else form_getrb:=0;
  end;


function form_findrb(von,bis,dn : byte) : byte;
  var q : byte;

  begin
    if von>bis then
      begin
        q:=von;
        von:=bis;
        bis:=q;
      end;
    form_findrb:=F_NONE;
    if bis<=_wmax then
      for q:=bis downto von do
        if (work[q,1]=#255) and (work[q,2]=chr(do_rnum)) and (work[q,5]=chr(dn)) and (work[q,6]=#1) then form_findrb:=q;
  end;


procedure form_setan(wnum : byte; cont : string);

  begin
    if wnum<=_wmax then
      if (work[wnum,1]=#255) and (work[wnum,2]=chr(do_anum)) and (length(work[wnum])>7) then
        work[wnum]:=left(left(work[wnum],8)+copy(work[wnum],9,ord(work[wnum,8]))+cont,90);
  end;


function form_getan(wnum : byte) : string;

  begin
    if wnum<=_wmax then
      begin
        if (work[wnum,1]=#255) and (work[wnum,2]=chr(do_anum)) and (length(work[wnum])>7) then
          form_getan:=right(work[wnum],length(work[wnum])-8-ord(work[wnum,8]))
        else form_getan:='';
      end
    else form_getan:='';
  end;


procedure form_setans(wnum,ds : byte);

  begin
    if wnum<=_wmax then
      if (work[wnum,1]=#255) and (work[wnum,2]=chr(do_anum)) and (length(work[wnum])>7) then
        begin
          if ds<>B_DISABLED then ds:=B_ENABLED;
          work[wnum,7]:=chr(ds);
        end;
  end;


function form_getans(wnum : byte) : byte;

  begin
    if wnum<=_wmax then
      begin
        if (work[wnum,1]=#255) and (work[wnum,2]=chr(do_anum)) and (length(work[wnum])>7) then
          form_getans:=ord(work[wnum,7])
        else form_getans:=0;
      end
    else form_getans:=0;
  end;


procedure pushprint(wnum,x,y : byte; var statchar : char);

  begin
    case statchar of
      ' ': begin
             statchar:='X';
             work[wnum,5]:=#1;
           end;
      'X': begin
             statchar:=' ';
             work[wnum,5]:=#0;
           end;
      '-': if work[wnum,5]=#2 then
             begin
               statchar:='X';
               work[wnum,5]:=#1;
             end
           else
             begin
               statchar:=' ';
               work[wnum,5]:=#0;
             end;
    end;
    printxy(succ(x),y,statchar);
    gotoxy(succ(x),y);
    repeat until mouseb=0;
  end;


procedure pushbutton(wnum,com,dat1,dat2 : byte);
  var txto,dummy,x,y : byte;
      statchar,hkey  : char;
      title          : string;

  begin
    txto:=textattr;
    x:=byteout[2]+ord(work[wnum,3]);
    y:=byteout[3]+ord(work[wnum,4]);
    case ord(work[wnum,5]) of
        0: statchar:=' ';
        1: statchar:='X';
      2,3: statchar:='-';
      255: statchar:=#255;
    end;
    title:=right(work[wnum],length(work[wnum])-5);
    dummy:=pos('#',title);
    if (dummy>0) and (dummy<length(title)) then
      begin
        hkey:=title[succ(dummy)];
        delete(title,dummy,1);
      end
    else hkey:=#0;
    textattr:=saw_dia;
    case com of
      0: begin
           if statchar=#255 then
             begin
               if farbe then textattr:=(haw_dia and 240)+darkgray
                 else textattr:=haw_dia;
             end;
           printxy(x,y,'['+statchar+'] '+title);
           if (hkey<>#0) and (statchar<>#255) then
             begin
               textattr:=haw_dia;
               printxy(x+3+dummy,y,hkey);
             end;
         end;
      1: if statchar=#255 then for dummy:=0 to 4 do byteout[dummy]:=0
         else
           begin
             byteout[0]:=ord(work[wnum,3]);
             byteout[1]:=ord(work[wnum,4]);
             byteout[2]:=byteout[0]+3+length(title);
             byteout[3]:=byteout[1];
             byteout[4]:=0;
           end;
      2: begin
           pushprint(wnum,x,y,statchar);
           byteout[3]:=0;
         end;
      3: if statchar=#255 then
           begin
             if byteout[4]=0 then byteout[3]:=succ(wnum)
               else byteout[3]:=pred(wnum);
             if byteout[3]<rbv then byteout[3]:=255;
           end
         else
           begin
             gotoxy(succ(x),y);
             byteout[3]:=0;
           end;
      5: if (upchar(chr(dat2))=upchar(hkey)) and (statchar<>#255) then byteout[0]:=1
           else byteout[0]:=0;
      10: begin
            if dat1=KEY_NORM then
              begin
                if (upchar(chr(dat2))=upchar(hkey)) or (dat2=32) then pushprint(wnum,x,y,statchar);
              end
            else
              begin
                if (dat2=72) or (dat2=75) then
                  begin
                    statchar:=' ';
                    pushprint(wnum,x,y,statchar);
                  end
                else
                  if (dat2=77) or (dat2=80) then
                    begin
                      statchar:='X';
                      pushprint(wnum,x,y,statchar);
                    end;
              end;
            byteout[3]:=0;
          end;
      11: begin
            pushprint(wnum,x,y,statchar);
            byteout[3]:=0;
          end;
      99: rbv:=dat1;
    end;
    textattr:=txto;
  end;


procedure radioprint(wnum,x,y : byte; var statchar : char);
  var q   : byte;
      num : char;

  begin
    statchar:=#7;
    work[wnum,6]:=#1;
    num:=work[wnum,5];
    for q:=rbv to rbb do
      if (q<>wnum) and (work[q,1]=#255) and (work[q,2]=chr(do_rnum)) and (work[q,5]=num) and (work[q,6]<>#255) then
        begin
          work[q,6]:=#0;
          printxy(byteout[2]+ord(work[q,3])+1,byteout[3]+ord(work[q,4]),' ');
        end;
    printxy(succ(x),y,statchar);
    gotoxy(succ(x),y);
    repeat until mouseb=0;
  end;


procedure radiobutton(wnum,com,dat1,dat2 : byte);
  var txto,dummy,x,y,q,n,fst,gef : byte;
      statchar,hkey,num          : char;
      title                      : string;

  begin
    txto:=textattr;
    x:=byteout[2]+ord(work[wnum,3]);
    y:=byteout[3]+ord(work[wnum,4]);
    case ord(work[wnum,6]) of
        0: statchar:=' ';
        1: statchar:=#7;
      255: statchar:=#255;
    end;
    title:=right(work[wnum],length(work[wnum])-6);
    dummy:=pos('#',title);
    if (dummy>0) and (dummy<length(title)) then
      begin
        hkey:=title[succ(dummy)];
        delete(title,dummy,1);
      end
    else hkey:=#0;
    textattr:=saw_dia;
    case com of
      0: begin
           if statchar=#255 then
             begin
               if farbe then textattr:=(haw_dia and 240)+darkgray
                 else textattr:=haw_dia;
             end;
           printxy(x,y,'('+statchar+') '+title);
           if (hkey<>#0) and (statchar<>#255) then
             begin
               textattr:=haw_dia;
               printxy(x+3+dummy,y,hkey);
             end;
         end;
      1: if statchar=#255 then for q:=0 to 4 do byteout[q]:=0
         else
           begin
             byteout[0]:=ord(work[wnum,3]);
             byteout[1]:=ord(work[wnum,4]);
             byteout[2]:=byteout[0]+3+length(title);
             byteout[3]:=byteout[1];
             byteout[4]:=0;
           end;
      2: begin
           radioprint(wnum,x,y,statchar);
           byteout[3]:=0;
         end;
      3: begin
           if (work[wnum,6]=#0) or (work[wnum,6]=#255) then
             begin
               if byteout[4]=0 then byteout[3]:=succ(wnum)
                 else byteout[3]:=pred(wnum);
               if byteout[3]<rbv then byteout[3]:=255;
             end
           else
             begin
               gotoxy(succ(x),y);
               byteout[3]:=0;
             end;
         end;
      5: if (upchar(chr(dat2))=upchar(hkey)) and (statchar<>#255) then byteout[0]:=1
           else byteout[0]:=0;
      10: if dat1=KEY_SPEC then
            begin
              if (dat2=72) or (dat2=75) then
                begin
                  q:=wnum;
                  gef:=0;
                  num:=work[wnum,5];
                  while gef=0 do
                    begin
                      dec(q);
                      if q<rbv then q:=rbb;
                      if (work[q,2]=chr(do_rnum)) and (work[q,5]=num) and (work[q,6]<>#255) then gef:=q;
                    end;
                  if gef<>wnum then
                    begin
                      radioprint(gef,byteout[2]+ord(work[gef,3]),byteout[3]+ord(work[gef,4]),statchar);
                      byteout[3]:=gef;
                      byteout[4]:=1;
                    end;
                end
              else
                if (dat2=77) or (dat2=80) then
                  begin
                    q:=wnum;
                    gef:=0;
                    num:=work[wnum,5];
                    while gef=0 do
                      begin
                        inc(q);
                        if q>rbb then q:=rbv;
                        if (work[q,2]=chr(do_rnum)) and (work[q,5]=num) and (work[q,6]<>#255) then gef:=q;
                      end;
                    if gef<>wnum then
                      begin
                        radioprint(gef,byteout[2]+ord(work[gef,3]),byteout[3]+ord(work[gef,4]),statchar);
                        byteout[3]:=gef;
                        byteout[4]:=0;
                      end;
                  end
                else byteout[3]:=0;
            end
          else byteout[3]:=0;
      11: begin
            repeat until mouseb=0;
            byteout[3]:=0;
          end;
      99: begin
            rbv:=dat1;
            rbb:=dat2;
            for n:=0 to 255 do
              begin
                num:=chr(n);
                fst:=0;
                gef:=0;
                for q:=rbv to rbb do
                  if (work[q,1]=#255) and (work[q,2]=chr(do_rnum)) and (work[q,5]=num) and (work[q,6]<>#255) then
                    begin
                      if fst=0 then fst:=q;
                      if work[q,6]=#1 then gef:=1;
                    end;
                if (fst>0) and (gef=0) then work[fst,6]:=#1;
              end;
          end;
    end;
    textattr:=txto;
  end;


procedure alphainp(wnum,com,dat1,dat2 : byte);
  var txto,mx,my,mb,x,y,ds : byte;
      fanf,br,mbr,dummy    : byte;
      hkey                 : char;
      title,anstr          : string;

  begin
    txto:=textattr;
    x:=byteout[2]+ord(work[wnum,3]);
    y:=byteout[3]+ord(work[wnum,4]);
    br:=ord(work[wnum,5]);
    mbr:=ord(work[wnum,6]);
    ds:=ord(work[wnum,7]);
    title:=copy(work[wnum],9,ord(work[wnum,8]));
    dummy:=pos('#',title);
    if (dummy>0) and (dummy<length(title)) then
      begin
        hkey:=title[succ(dummy)];
        delete(title,dummy,1);
      end
    else hkey:=#0;
    anstr:=form_getan(wnum);
    fanf:=x+length(title)+2;
    textattr:=saw_dia;
    case com of
      0: begin
           if mbr<2 then
             begin
               mbr:=2;
               work[wnum,6]:=#2;
             end;
           if br<2 then
             begin
               br:=2;
               work[wnum,5]:=#2;
             end;
           if length(anstr)>mbr then
             begin
               anstr:=left(anstr,mbr);
               form_setan(wnum,anstr);
             end;
           if ds=B_DISABLED then
             begin
               if farbe then textattr:=(haw_dia and 240)+darkgray
                 else textattr:=haw_dia;
             end;
           printxy(x,y,title+' ['+strg('.',br)+']');
           printxy(fanf,y,left(anstr,br));
           if (hkey<>#0) and (ds<>B_DISABLED) then
             begin
               textattr:=haw_dia;
               printxy(pred(x+dummy),y,hkey);
             end;
         end;
      1: if ds=B_DISABLED then for dummy:=0 to 4 do byteout[dummy]:=0
         else
           begin
             byteout[0]:=ord(work[wnum,3])+length(title)+2;
             byteout[1]:=ord(work[wnum,4]);
             byteout[2]:=pred(byteout[0]+br);
             byteout[3]:=byteout[1];
             byteout[4]:=1;
           end;
    2,3: if ds=B_DISABLED then
           begin
             if byteout[4]=0 then byteout[3]:=succ(wnum)
               else byteout[3]:=pred(wnum);
             if byteout[3]<rbv then byteout[3]:=255;
           end
         else
           begin
             if (com=2) and (dat1>0) then
               begin
                 ano:=0;
                 anp:=dat1-fanf;
                 tstat:=false;
               end
             else
               begin
                 if length(anstr)>=br then
                   begin
                     ano:=succ(length(anstr)-br);
                     printxy(fanf,y,right(anstr,pred(br))+'.');
                     setattr(fanf,y,weissaufschwarz,pred(br));
                   end
                 else
                   begin
                     ano:=0;
                     setattr(fanf,y,weissaufschwarz,length(anstr));
                   end;
                 anp:=pred(br);
                 if length(anstr)>0 then tstat:=true else tstat:=false;
               end;
             if ano+anp>length(anstr) then anp:=length(anstr)-ano;
             if tstat then hide else gotoxy(fanf+anp,y);
             byteout[3]:=0;
           end;
      5: if (upchar(chr(dat2))=upchar(hkey)) and (ds<>B_DISABLED) then byteout[0]:=1
           else byteout[0]:=0;
      7: if ds<>B_DISABLED then
           begin
             printxy(fanf,y,strg('.',br));
             printxy(fanf,y,left(anstr,br));
             tstat:=false;
           end;
      10: begin
            if dat1=KEY_SPEC then
              case dat2 of
                71: begin
                      ano:=0;
                      anp:=0;
                      printxy(fanf,y,strg('.',br));
                      printxy(fanf,y,left(anstr,br));
                      gotoxy(fanf,y);
                      tstat:=false;
                    end;
                79: begin
                      if length(anstr)>=br then ano:=succ(length(anstr)-br) else ano:=0;
                      printxy(fanf,y,right(anstr,pred(br))+'.');
                      anp:=pred(br);
                      if ano+anp>length(anstr) then anp:=length(anstr)-ano;
                      gotoxy(fanf+anp,y);
                      tstat:=false;
                    end;
                75: if tstat then
                      begin
                        if length(anstr)>=br then setattr(fanf,y,saw_dia,pred(br))
                          else setattr(fanf,y,saw_dia,length(anstr));
                        dec(anp);
                        gotoxy(fanf+anp,y);
                        tstat:=false;
                      end
                    else
                      begin
                        if anp>0 then
                          begin
                            dec(anp);
                            gotoxy(fanf+anp,y);
                          end
                        else
                          if ano>0 then
                            begin
                              printxy(fanf,y,copy(anstr,ano,br));
                              dec(ano);
                            end;
                      end;
                77: if tstat then
                      begin
                        if length(anstr)>=br then setattr(fanf,y,saw_dia,pred(br))
                          else setattr(fanf,y,saw_dia,length(anstr));
                        gotoxy(fanf+anp,y);
                        tstat:=false;
                      end
                    else
                      if ano+anp<length(anstr) then
                        begin
                          inc(anp);
                          if anp>pred(br) then
                            begin
                              inc(ano);
                              anp:=pred(br);
                              if ano>length(anstr)-br then printxy(fanf,y,copy(anstr,succ(ano),pred(br))+'.')
                                else printxy(fanf,y,copy(anstr,succ(ano),br));
                            end;
                          if ano+anp>length(anstr) then anp:=length(anstr)-ano;
                          gotoxy(fanf+anp,y);
                        end;
                83: if tstat then
                      begin
                        ano:=0;
                        anp:=0;
                        printxy(fanf,y,strg('.',br));
                        form_setan(wnum,'');
                        gotoxy(fanf,y);
                        tstat:=false;
                      end
                    else
                      if ano+anp<length(anstr) then
                        begin
                          anstr:=left(anstr,ano+anp)+right(anstr,pred(length(anstr)-ano-anp));
                          if ano>length(anstr)-br then printxy(fanf,y,copy(anstr,succ(ano),pred(br))+'.')
                            else printxy(fanf,y,copy(anstr,succ(ano),br));
                          form_setan(wnum,anstr);
                        end;
              end
            else
              case dat2 of
                 8: if tstat then
                      begin
                        ano:=0;
                        anp:=0;
                        printxy(fanf,y,strg('.',br));
                        form_setan(wnum,'');
                        gotoxy(fanf,y);
                        tstat:=false;
                      end
                    else
                      if ano+anp>0 then
                        begin
                          if anp=0 then dec(ano) else dec(anp);
                          if (anp=0) and (ano>0) then
                            begin
                              dec(ano);
                              anp:=1;
                            end;
                          anstr:=left(anstr,ano+anp)+right(anstr,pred(length(anstr)-ano-anp));
                          if ano>length(anstr)-br then printxy(fanf,y,copy(anstr,succ(ano),pred(br))+'.')
                            else printxy(fanf,y,copy(anstr,succ(ano),br));
                          form_setan(wnum,anstr);
                          gotoxy(fanf+anp,y);
                        end;
                25: begin
                      ano:=0;
                      anp:=0;
                      printxy(fanf,y,strg('.',br));
                      form_setan(wnum,'');
                      gotoxy(fanf,y);
                      tstat:=false;
                    end;
              else
                begin
                  if tstat then
                    begin
                      if dat2>31 then
                        begin
                          ano:=0;
                          anp:=1;
                          printxy(fanf,y,chr(dat2)+strg('.',pred(br)));
                          form_setan(wnum,chr(dat2));
                          gotoxy(succ(fanf),y);
                          tstat:=false;
                        end;
                    end
                  else
                    if ((length(anstr)<mbr) or (ano+anp<mbr)) and (dat2>31) then
                      begin
                        anstr:=left(left(anstr,ano+anp)+chr(dat2)+right(anstr,length(anstr)-ano-anp),mbr);
                        if anp<pred(br) then inc(anp) else inc(ano);
                        if ano>length(anstr)-br then printxy(fanf,y,copy(anstr,succ(ano),pred(br))+'.')
                          else printxy(fanf,y,copy(anstr,succ(ano),br));
                        form_setan(wnum,anstr);
                        gotoxy(fanf+anp,y);
                      end;
                end;
              end;
            byteout[3]:=0;
          end;
      11: begin
            if tstat then
              begin
                if length(anstr)>=br then setattr(fanf,y,saw_dia,pred(br))
                  else setattr(fanf,y,saw_dia,length(anstr));
                tstat:=false;
              end;
            showmouse;
            anp:=dat1-fanf;
            if ano+anp>length(anstr) then anp:=length(anstr)-ano;
            gotoxy(fanf+anp,y);
            repeat
              getmouse(mx,my,mb);
              if mx<pred(fanf) then
                begin
                  anp:=0;
                  if ano>0 then
                    begin
                      printxy(fanf,y,copy(anstr,ano,br));
                      dec(ano);
                      delay(50);
                    end;
                  gotoxy(fanf,y);
                end;
              if (mx>fanf+br) then
                begin
                  anp:=pred(br);
                  if ano<=length(anstr)-br then
                    begin
                      inc(ano);
                      if ano>length(anstr)-br then printxy(fanf,y,copy(anstr,succ(ano),pred(br))+'.')
                        else printxy(fanf,y,copy(anstr,succ(ano),br));
                      delay(50);
                    end;
                  if ano+anp>length(anstr) then anp:=length(anstr)-ano;
                  gotoxy(fanf+anp,y);
                end;
            until mb=0;
            byteout[3]:=0;
            hidemouse;
          end;
      99: rbv:=dat1;
    end;
    textattr:=txto;
  end;