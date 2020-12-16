{ ASV 1.3 ALRT vom 20.04.1991 }

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
    if stat>M_DROP then stat:=M_PULL;
    mpd_stat:=stat;
  end;