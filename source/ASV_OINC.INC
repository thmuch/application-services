{ ASVOBJ 1.0 OINC vom 20.04.1991 }

procedure form_setslsdelay(dl : word);

  begin
    sldelay:=dl;
  end;


function form_getslsdelay : word;

  begin
    form_getslsdelay:=sldelay;
  end;


procedure form_setslmdelay(dl : word);

  begin
    slmdelay:=dl;
  end;


function form_getslmdelay : word;

  begin
    form_getslmdelay:=slmdelay;
  end;


procedure obj_setcolor(ws,hs,sw,hw : byte);

  begin
    was:=ws;  has:=hs;
    saw:=sw;  haw:=hw;
  end;


function form_getslidermax(wnum : byte) : word;

  begin
    if wnum<=_wmax then
      if (work[wnum,1]=#255) and ((work[wnum,2]=chr(hslnum)) or (work[wnum,2]=chr(vslnum)))
        and (length(work[wnum])>8) then form_getslidermax:=ord(work[wnum,8])*256+ord(work[wnum,9])
      else form_getslidermax:=0;
  end;


procedure form_setslider(wnum : byte; wert : word);
  var max : word;

  begin
    if wnum<=_wmax then
      if (work[wnum,1]=#255) and ((work[wnum,2]=chr(hslnum)) or (work[wnum,2]=chr(vslnum))) and (length(work[wnum])>8) then
        begin
          max:=form_getslidermax(wnum);
          if wert>max then wert:=max;
          work[wnum,6]:=chr(hi(wert));
          work[wnum,7]:=chr(lo(wert));
        end;
  end;


function form_getslider(wnum : byte) : word;

  begin
    if wnum<=_wmax then
      if (work[wnum,1]=#255) and ((work[wnum,2]=chr(hslnum)) or (work[wnum,2]=chr(vslnum)))
        and (length(work[wnum])>8) then form_getslider:=ord(work[wnum,6])*256+ord(work[wnum,7])
      else form_getslider:=0;
  end;


procedure form_setslidermax(wnum : byte; max : word);

  begin
    if wnum<=_wmax then
      if (work[wnum,1]=#255) and ((work[wnum,2]=chr(hslnum)) or (work[wnum,2]=chr(vslnum))) and (length(work[wnum])>8) then
        begin
          work[wnum,8]:=chr(hi(max));
          work[wnum,9]:=chr(lo(max));
          if form_getslider(wnum)>max then form_setslider(wnum,max);
        end;
  end;


function dhslider(x,y,brt : byte; wert,max : word) : string;

  begin
    if brt<2 then brt:=2;
    if wert>max then wert:=max;
    dhslider:=#255+chr(hslnum)+chr(x)+chr(y)+chr(brt)+chr(hi(wert))+chr(lo(wert))+chr(hi(max))+chr(lo(max));
  end;


function dvslider(x,y,hhe : byte; wert,max : word) : string;

  begin
    if hhe<2 then hhe:=2;
    if wert>max then wert:=max;
    dvslider:=#255+chr(vslnum)+chr(x)+chr(y)+chr(hhe)+chr(hi(wert))+chr(lo(wert))+chr(hi(max))+chr(lo(max));
  end;


function _hslider : integer;

  begin
    _hslider:=hslnum;
  end;


function _vslider : integer;

  begin
    _vslider:=vslnum;
  end;


procedure verslider(wnum,com,dat1,dat2 : byte);
  label sl_nmove;

  var dtxta,q,x,y,ybis : byte;
      mx,my,mb,yo,hhe  : byte;
      wert,max,wdif    : word;

  begin
    dtxta:=textattr;
    x:=byteout[2]+ord(work[wnum,3]);
    y:=byteout[3]+ord(work[wnum,4]);
    hhe:=ord(work[wnum,5]);
    if hhe<2 then
      begin
        hhe:=2;
        work[wnum,5]:=#2;
      end;
    ybis:=pred(y+hhe);
    wert:=ord(work[wnum,6])*256+ord(work[wnum,7]);
    max:=ord(work[wnum,8])*256+ord(work[wnum,9]);
    textattr:=saw;
    case com of
      0: begin
           printxy(x,y,#24);
           printxy(x,ybis,#25);
           if hhe>2 then
             begin
               for q:=succ(y) to pred(ybis) do printxy(x,q,'°');
               textattr:=was;
               if max>0 then printxy(x,succ(y+trunc((hhe-3)*wert/max)),#18)
                 else printxy(x,succ(y),' ');
             end;
         end;
      1: begin
           byteout[0]:=ord(work[wnum,3]);
           byteout[1]:=ord(work[wnum,4]);
           byteout[2]:=byteout[0];
           byteout[3]:=pred(byteout[1]+hhe);
           byteout[4]:=0;
         end;
   2,11: begin
           if max=0 then
             begin
               repeat until mouseb=0;
               goto sl_nmove;
             end;
           yo:=succ(y+trunc((hhe-3)*wert/max));
           if dat2=y then
             begin
               if wert>0 then
                 begin
                   if hhe>2 then printxy(x,yo,'°');
                   dec(wert);
                   form_setslider(wnum,wert);
                   textattr:=was;
                   if hhe>2 then printxy(x,succ(y+trunc((hhe-3)*wert/max)),#18);
                   delay(sldelay);
                 end
               else repeat until mouseb=0;
             end
           else
             if dat2=ybis then
               begin
                 if wert<max then
                   begin
                     if hhe>2 then printxy(x,yo,'°');
                     inc(wert);
                     form_setslider(wnum,wert);
                     textattr:=was;
                     if hhe>2 then printxy(x,succ(y+trunc((hhe-3)*wert/max)),#18);
                     delay(sldelay);
                   end
                 else repeat until mouseb=0;
               end
             else
               if hhe>3 then
                 begin
                   wdif:=max div (hhe-3);
                   if (dat2>y) and (dat2<yo) then
                     begin
                       printxy(x,yo,'°');
                       if wdif>wert then wert:=0
                         else dec(wert,wdif);
                       form_setslider(wnum,wert);
                       textattr:=was;
                       printxy(x,succ(y+trunc((hhe-3)*wert/max)),#18);
                       delay(slmdelay);
                     end
                   else
                     if (dat2>yo) and (dat2<ybis) then
                       begin
                         printxy(x,yo,'°');
                         if (wert+wdif)>max then wert:=max
                           else inc(wert,wdif);
                         form_setslider(wnum,wert);
                         textattr:=was;
                         printxy(x,succ(y+trunc((hhe-3)*wert/max)),#18);
                         delay(slmdelay);
                       end
                     else
                       if dat2=yo then
                         begin
                           textattr:=was;
                           printxy(x,yo,'þ');
                           repeat
                             getmouse(mx,my,mb);
                             if my<succ(y) then my:=succ(y);
                             if my>pred(ybis) then my:=pred(ybis);
                             if (my<>yo) then
                               begin
                                 textattr:=saw;
                                 printxy(x,yo,'°');
                                 yo:=my;
                                 textattr:=was;
                                 printxy(x,yo,'þ');
                               end;
                           until mb=0;
                           wert:=pred(yo-y)*wdif;
                           while succ(y+trunc((hhe-3)*wert/max))<yo do inc(wert);
                           form_setslider(wnum,wert);
                           textattr:=saw;
                           printxy(x,yo,'°');
                           textattr:=was;
                           printxy(x,succ(y+trunc((hhe-3)*wert/max)),#18);
                         end;
                 end;
           sl_nmove:
           byteout[3]:=0;
           hide;
         end;
      3: if byteout[4]=0 then byteout[3]:=succ(wnum)
           else byteout[3]:=pred(wnum);
      5: byteout[0]:=0;
     10: byteout[3]:=0;
    end;
    textattr:=dtxta;
  end;


procedure horslider(wnum,com,dat1,dat2 : byte);
  label sl_nmove;

  var dtxta,q,x,y,xbis : byte;
      mx,my,mb,brt,xo  : byte;
      wert,max,wdif    : word;

  begin
    dtxta:=textattr;
    x:=byteout[2]+ord(work[wnum,3]);
    y:=byteout[3]+ord(work[wnum,4]);
    brt:=ord(work[wnum,5]);
    if brt<2 then
      begin
        brt:=2;
        work[wnum,5]:=#2
      end;
    xbis:=pred(x+brt);
    wert:=ord(work[wnum,6])*256+ord(work[wnum,7]);
    max:=ord(work[wnum,8])*256+ord(work[wnum,9]);
    textattr:=saw;
    case com of
      0: begin
           printxy(x,y,#27);
           printxy(xbis,y,#26);
           if brt>2 then
             begin
               printxy(succ(x),y,strg('°',brt-2));
               textattr:=was;
               if max>0 then printxy(succ(x+trunc((brt-3)*wert/max)),y,#29)
                 else printxy(succ(x),y,' ');
             end;
         end;
      1: begin
           byteout[0]:=ord(work[wnum,3]);
           byteout[1]:=ord(work[wnum,4]);
           byteout[2]:=pred(byteout[0]+brt);
           byteout[3]:=byteout[1];
           byteout[4]:=0;
         end;
   2,11: begin
           if max=0 then
             begin
               repeat until mouseb=0;
               goto sl_nmove;
             end;
           xo:=succ(x+trunc((brt-3)*wert/max));
           if dat1=x then
             begin
               if wert>0 then
                 begin
                   if brt>2 then printxy(xo,y,'°');
                   dec(wert);
                   form_setslider(wnum,wert);
                   textattr:=was;
                   if brt>2 then printxy(succ(x+trunc((brt-3)*wert/max)),y,#29);
                   delay(sldelay);
                 end
               else repeat until mouseb=0;
             end
           else
             if dat1=xbis then
               begin
                 if wert<max then
                   begin
                     if brt>2 then printxy(xo,y,'°');
                     inc(wert);
                     form_setslider(wnum,wert);
                     textattr:=was;
                     if brt>2 then printxy(succ(x+trunc((brt-3)*wert/max)),y,#29);
                     delay(sldelay);
                   end
                 else repeat until mouseb=0;
               end
             else
               if brt>3 then
                 begin
                   wdif:=max div (brt-3);
                   if (dat1>x) and (dat1<xo) then
                     begin
                       printxy(xo,y,'°');
                       if wdif>wert then wert:=0
                         else dec(wert,wdif);
                       form_setslider(wnum,wert);
                       textattr:=was;
                       printxy(succ(x+trunc((brt-3)*wert/max)),y,#29);
                       delay(slmdelay);
                     end
                   else
                     if (dat1>xo) and (dat1<xbis) then
                       begin
                         printxy(xo,y,'°');
                         if (wert+wdif)>max then wert:=max
                           else inc(wert,wdif);
                         form_setslider(wnum,wert);
                         textattr:=was;
                         printxy(succ(x+trunc((brt-3)*wert/max)),y,#29);
                         delay(slmdelay);
                       end
                     else
                       if dat1=xo then
                         begin
                           textattr:=was;
                           printxy(xo,y,'þ');
                           repeat
                             getmouse(mx,my,mb);
                             if mx<succ(x) then mx:=succ(x);
                             if mx>pred(xbis) then mx:=pred(xbis);
                             if (mx<>xo) then
                               begin
                                 textattr:=saw;
                                 printxy(xo,y,'°');
                                 xo:=mx;
                                 textattr:=was;
                                 printxy(xo,y,'þ');
                               end;
                           until mb=0;
                           wert:=pred(xo-x)*wdif;
                           while succ(x+trunc((brt-3)*wert/max))<xo do inc(wert);
                           form_setslider(wnum,wert);
                           textattr:=saw;
                           printxy(xo,y,'°');
                           textattr:=was;
                           printxy(succ(x+trunc((brt-3)*wert/max)),y,#29);
                         end;
                 end;
           sl_nmove:
           byteout[3]:=0;
           hide;
         end;
      3: if byteout[4]=0 then byteout[3]:=succ(wnum)
           else byteout[3]:=pred(wnum);
      5: byteout[0]:=0;
     10: byteout[3]:=0;
    end;
    textattr:=dtxta;
  end;


function stn(var strng : string; ch : char) : integer;
  var p : byte;

  begin
    p:=pos(ch,strng);
    if p>0 then
      begin
        stn:=atoi(left(strng,pred(p)));
        strng:=right(strng,length(strng)-p);
      end
    else
      begin
        stn:=atoi(strng);
        strng:='';
      end;
  end;