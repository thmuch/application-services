{ ASV 1.5 COLR vom 20.12.1991 }

procedure coldel(x,y,atr : byte);

  begin
    textattr:=saw_dia;
    printxy(pred(x),y+(atr div 32),'³');
    printxy(x+32,y+(atr div 32),'³');
    printxy(x+(atr mod 32),y+8,'Ä');
  end;


procedure colprnt(x,y,atr,arw : byte);

  begin
    textattr:=atr;
    printxy(x+7,y+10,'ABCabc 123.!?');
    textattr:=arw;
    printxy(pred(x),y+(atr div 32),#16);
    printxy(x+32,y+(atr div 32),#17);
    printxy(x+(atr mod 32),y+8,#30);
    textattr:=saw_dia;
    printxy(x+21,y+10,'('+itoa(atr)+')  ');
  end;


procedure colobj(wnum,com,dat1,dat2 : byte);
  var dtxta,q,w,x,y : byte;

  begin
    x:=byteout[2]+4;
    y:=byteout[3]+3;
    dtxta:=textattr;
    case com of
      0: begin
           textattr:=0;
           for w:=0 to 7 do
             for q:=0 to 31 do
               begin
                 printxy(x+q,y+w,'?');
                 inc(textattr);
               end;
           colprnt(x,y,ord(work[wnum,3]),saw_dia);
         end;
      1: begin
           byteout[0]:=4;
           byteout[1]:=3;
           byteout[2]:=35;
           byteout[3]:=10;
           byteout[4]:=0;
         end;
      2,11: begin
              q:=(dat1-x)+(dat2-y)*32;
              if q=ord(work[wnum,3]) then repeat until (mousex<>dat1) or (mousey<>dat2) or (mouseb=0);
              coldel(x,y,ord(work[wnum,3]));
              work[wnum,3]:=chr(q);
              colprnt(x,y,q,haw_dia);
              byteout[3]:=0;
            end;
      3: begin
           colprnt(x,y,ord(work[wnum,3]),haw_dia);
           byteout[3]:=0;
         end;
      5: byteout[0]:=0;
      7: colprnt(x,y,ord(work[wnum,3]),saw_dia);
     10: begin
           if dat1=KEY_SPEC then
             begin
               q:=ord(work[wnum,3]);
               case dat2 of
                 K_RIGHT: if q<255 then inc(q) else q:=0;
                 K_LEFT:  if q>0 then dec(q) else q:=255;
                 K_UP:    if (q div 32)>0 then dec(q,32) else inc(q,224);
                 K_DOWN:  if (q div 32)<7 then inc(q,32) else dec(q,224);
               end;
               coldel(x,y,ord(work[wnum,3]));
               work[wnum,3]:=chr(q);
               colprnt(x,y,q,haw_dia);
             end;
           byteout[3]:=0;
         end;
    end;
    textattr:=dtxta;
  end;


function colours(title : string; attr : byte) : integer;
  var but,conum : integer;
      worksave  : array [0..9] of string[90];
      q         : byte;

  begin
    conum:=form_objins(colobj);
    if conum=F_ERROR then
      begin
        colours:=-1;
        exit;
      end;
    for q:=0 to 9 do worksave[q]:=work[q];
    work[0]:=chr(clx)+chr(cly)+#0#0#40#18;
    work[1]:=left(title,28);
    if farbe then
      begin
        case ccode of
          L_USA: work[2]:=dframe(3,2,34,10,'Colors');
          L_GB:  work[2]:=dframe(3,2,34,10,'Colours');
        else work[2]:=dframe(3,2,34,10,'Farben');
        end;
      end
    else
      begin
        if (ccode=L_USA) or (ccode=L_GB) then work[2]:=dframe(3,2,34,10,'Attributes')
          else work[2]:=dframe(3,2,34,10,'Attribute');
      end;
    work[3]:=dtext(5,13,'Test:');
    work[4]:='';
    work[5]:=#255+chr(conum)+chr(attr);
    work[6]:='  #OK  ';
    if (ccode=L_USA) or (ccode=L_GB) then work[7]:='#Cancel'
      else work[7]:='#Abbruch';
    work[8]:='';
    work[9]:='';
    but:=dialog;
    clx:=byteout[0];
    cly:=byteout[1];
    if but=0 then colours:=ord(work[5,3])
      else colours:=-1;
    void.i:=form_objdel(conum);
    for q:=0 to 9 do work[q]:=worksave[q];
  end;