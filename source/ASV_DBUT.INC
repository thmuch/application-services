{ ASV 1.5 DBUT vom 21.12.1991 }

function dbut_getname(name : string) : string;
  var attr : byte;
      nme  : string;

  begin
    attr:=weissaufschwarz;
    name:=left(name,75);
    nme:='';
    while length(name)>0 do
      begin
        if (name[1]='#') and (length(name)>1) then
          begin
            attr:=ord(name[2]);
            name:=right(name,length(name)-2);
            if attr=0 then attr:=weissaufschwarz;
          end
        else
          begin
            nme:=nme+name[1]+chr(attr);
            cutl(name);
          end;
      end;
    dbut_getname:=left(nme,50);
  end;


procedure dbut_prnt(num : byte);
  var dtxta,q   : byte;
      sofs,sseg : word;

  begin
    dtxta:=textattr;
    with dbuttons[num] do
      begin
        sseg:=seg(name);
        sofs:=succ(ofs(name));
        vofs:=(anzcol shl 1)*pred(y)+(pred(x) shl 1);
        for q:=1 to tlen do
          begin
            memw[vioseg:vofs]:=memw[sseg:sofs];
            inc(vofs,2);
            inc(sofs,2);
          end;
        textattr:=fcol;
        case frame of
          B_FSINGLE: begin
                       printxy(pred(x),pred(y),'Ú'+strg('Ä',tlen)+'¿');
                       printxy(pred(x),y,'³');
                       printxy(x+tlen,y,'³');
                       printxy(pred(x),succ(y),'À'+strg('Ä',tlen)+'Ù');
                     end;
          B_FHALF:   begin
                       printxy(pred(x),pred(y),'Õ'+strg('Í',tlen)+'¸');
                       printxy(pred(x),y,'³');
                       printxy(x+tlen,y,'³');
                       printxy(pred(x),succ(y),'Ô'+strg('Í',tlen)+'¾');
                     end;
          B_FDOUBLE: begin
                       printxy(pred(x),pred(y),'É'+strg('Í',tlen)+'»');
                       printxy(pred(x),y,'º');
                       printxy(x+tlen,y,'º');
                       printxy(pred(x),succ(y),'È'+strg('Í',tlen)+'¼');
                     end;
        end;
      end;
    if num=dbutak then dbutak:=255;
    textattr:=dtxta;
  end;


procedure dbut_draw(was : byte);
  var q : byte;

  begin
    hidemouse;
    if was=B_ALL then
      begin
        for q:=0 to _dbutmax do
          if dbuttons[q].stat=B_USED then dbut_prnt(q);
      end
    else
      begin
        if was<=_dbutmax then
          if dbuttons[was].stat=B_USED then dbut_prnt(was);
      end;
    showmouse;
  end;


function dbut_insert(x,y,frm,fcol,ext,menu,kshft,kcode : byte; name : string) : integer;
  var q,num,fflag : byte;

  begin
    num:=255;
    for q:=_dbutmax downto 0 do if dbuttons[q].stat=B_UNUSED then num:=q;
    if num=255 then
      begin
        dbut_insert:=B_ERROR;
        exit;
      end;
    if not(frm in [B_FNONE,B_FSINGLE,B_FHALF,B_FDOUBLE]) then frm:=B_FNONE;
    if not(ext in [B_CLICK,B_TOUCH]) then ext:=B_CLICK;
    with dbuttons[num] do
      begin
        frame:=frm;
        bexit:=ext;
        amenu:=menu;
        kashft:=kshft;
        kacode:=kcode;
      end;
    dbuttons[num].fcol:=fcol;
    dbuttons[num].name:=dbut_getname(name);
    if frm<>B_FNONE then fflag:=1 else fflag:=0;
    if (x-fflag<1) or (y-fflag<1) or (pred(x+(length(name) shr 1)+fflag)>anzcol) or (y+fflag>anzline) or
       (length(dbuttons[num].name)=0) then dbut_insert:=B_ERROR
    else
      begin
        dbuttons[num].x:=x;
        dbuttons[num].y:=y;
        dbuttons[num].tlen:=length(dbuttons[num].name) shr 1;
        dbuttons[num].stat:=B_USED;
        dbut_insert:=num;
      end;
  end;


function dbut_delete(num : byte) : integer;

  begin
    if num<=_dbutmax then
      begin
        if dbuttons[num].stat=B_USED then
          begin
            dbuttons[num].stat:=B_UNUSED;
            if num=dbutak then dbutak:=255;
            dbut_delete:=B_OK;
          end
        else dbut_delete:=B_ERROR;
      end
    else dbut_delete:=B_ERROR;
  end;


procedure dbut_update(num,frm,fcol,menu,kshft,kcode : byte; name : string);

  begin
    if num<=_dbutmax then
      if dbuttons[num].stat=B_USED then
        begin
          name:=dbut_getname(name);
          if length(name)=length(dbuttons[num].name) then dbuttons[num].name:=name;
          dbuttons[num].fcol:=fcol;
          if not(frm in [B_FNONE,B_FSINGLE,B_FHALF,B_FDOUBLE]) then frm:=B_FNONE;
          with dbuttons[num] do
            begin
              if frm<>B_FNONE then frame:=frm;
              amenu:=menu;
              kashft:=kshft;
              kacode:=kcode;
            end;
          hidemouse;
          dbut_prnt(num);
          showmouse;
        end;
  end;


function dbut_event(mx,my,mb : byte) : integer;
  var q : byte;

  begin
    dbut_event:=-12345;
    if dbutak=255 then
      begin
        for q:=0 to _dbutmax do
          with dbuttons[q] do
            if stat=B_USED then
              if (mx>=x) and (mx<x+tlen) and (my=y) then dbutak:=q;
        if dbutak<255 then
          with dbuttons[dbutak] do
            begin
              if (bexit=B_TOUCH) or ((mb and M_LEFT)=M_LEFT) then
                begin
                  hidemouse;
                  setattr(x,y,saw_dia,tlen);
                  showmouse;
                end
              else dbutak:=255;
            end;
      end
    else
      with dbuttons[dbutak] do
        begin
          if (mx<x) or (mx>=x+tlen) or (my<>y) then
            begin
              hidemouse;
              dbut_prnt(dbutak);
              showmouse;
            end
          else
            begin
              if ((bexit=B_TOUCH) and ((mb and M_LEFT)=M_LEFT)) or ((bexit=B_CLICK) and ((mb and M_LEFT)=0)) then
                begin
                  if amenu=B_MNONE then dbut_event:=B_OFFS+dbutak
                    else dbut_event:=amenu;
                  if (kashft in [KEY_NORM,KEY_SPEC]) and (kacode<>B_KNONE) then
                    begin
                      byteout[0]:=kashft;
                      byteout[1]:=kacode;
                    end;
                  hidemouse;
                  dbut_prnt(dbutak);
                  showmouse;
                end;
            end;
        end;
  end;