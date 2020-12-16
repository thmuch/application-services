{ ASV 1.5 ALRT vom 15.12.1991 }

function _dactive : byte;

  begin
    if dialog_aktiv then _dactive:=aktiv else _dactive:=D_NONE;
  end;


function dialog : integer;
  var q                   : integer;
      saw_dummy,haw_dummy : byte;

  begin
    if dialog_aktiv then dialog:=E_NREENT
    else
      begin
        dialog_aktiv:=true;
        for q:=0 to _wmax do dwork[q]:=work[q];
        saw_dummy:=schwarzaufweiss;
        haw_dummy:=hellaufweiss;
        schwarzaufweiss:=saw_dia;
        hellaufweiss:=haw_dia;
        dialog:=dialog_do;
        schwarzaufweiss:=saw_dummy;
        hellaufweiss:=haw_dummy;
        dialog_aktiv:=false;
      end;
  end;


function form_alert(title,msg,but : string; def : byte) : integer;
  var q,w,c,apos,bm       : integer;
      saw_dummy,haw_dummy : byte;

  begin
    if dialog_aktiv then
      begin
        form_alert:=E_NREENT;
        exit;
      end;
    dialog_aktiv:=true;
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
    if (dbits and D_CLOSE)=D_CLOSE then
      begin
        if bm<length(title)+10 then bm:=length(title)+10;
      end
    else
      begin
        if bm<length(title) then bm:=length(title);
      end;
    dwork[0]:=dwork[0]+chr(bm+4);
    if w=3 then dwork[0]:=dwork[0]+#3
      else dwork[0]:=dwork[0]+chr(w+3);
    dwork[0]:=dwork[0]+chr(byteout[4]);
    saw_dummy:=schwarzaufweiss;
    haw_dummy:=hellaufweiss;
    schwarzaufweiss:=saw_ale;
    hellaufweiss:=haw_ale;
    form_alert:=dialog_do;
    schwarzaufweiss:=saw_dummy;
    hellaufweiss:=haw_dummy;
    dialog_aktiv:=false;
  end;


procedure form_errexit(stat : byte);

  begin
    if stat>X_CANCEL then stat:=X_BOTH;
    eexit_stat:=stat;
  end;


procedure menu_sets(stat : byte);

  begin
    mpd_stat:=(stat and M_DROP);
    mpm_stat:=M_NMOVE-(stat and M_NMOVE);
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
                   'Resource file not found',
                   'Double function call not allowed',
                   'Menu not drawn yet',
                   'Menu out of screen',
                   'Menu still used',
                   'Menu not initialized',
                   'Button missing in dialog box',
                   'Error in dialog structure',
                   'Dialog box too big',
                   'Error in menu structure');

  var fehler   : integer;
      but,cbut : byte;
      odbits   : word;
      bstr     : string[16];

  begin
    fehler:=num;
    if num<=M_ERROR then
      begin
        if num<_emin then num:=0
          else num:=abs(num)-9;
        write(#7);
        odbits:=dbits;
        dbits:=D_MOVE;
        if eexit_stat=X_CANCEL then
          begin
            if (ccode=L_USA) or (ccode=L_GB) then bstr:=' #Quit '
              else bstr:='#Abbruch';
            cbut:=0;
          end
        else
          begin
            bstr:='  #OK  ';
            cbut:=1;
          end;
        if eexit_stat=X_BOTH then
          begin
            if (ccode=L_USA) or (ccode=L_GB) then bstr:=bstr+'| #Quit '
              else bstr:=bstr+'|#Abbruch';
          end;
        if (ccode=L_USA) or (ccode=L_GB) then but:=form_alert('ERROR','['+itoa(fehler)+']: '+errmsge[num],bstr,0)
          else but:=form_alert('FEHLER','['+itoa(fehler)+']: '+errmsgd[num],bstr,0);
        dbits:=odbits;
        if (but=cbut) or (eexit_stat=X_CANCEL) then
          begin
            hidemouse;
            writeln;
            write('  Runtime Error ',fehler,' [ASV]:  ');
            if (ccode=L_USA) or (ccode=L_GB) then writeln(errmsge[num]) else writeln(errmsgd[num]);
            writeln;
            halt;
          end;
      end;
  end;