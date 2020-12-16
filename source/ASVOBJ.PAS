{***********************************
 * ASV Objects 1.0 by Thomas Much  *
 * Copyright (c)1991 by Softdesign *
 ***********************************
 * entwickelt am:     16.02.1991   *
 * letztes Update am: 02.06.1991   *
 ***********************************}

unit asvobj;
{$A+,B-,D-,E-,F+,I-,L-,N-,O+,R-,S-,V+}

interface

  uses asv,dos,crt;

  procedure form_setslsdelay(dl : word);
  function form_getslsdelay : word;
  procedure form_setslmdelay(dl : word);
  function form_getslmdelay : word;
  procedure obj_setcolor(ws,hs,sw,hw : byte);
  function form_getslidermax(wnum : byte) : word;
  procedure form_setslider(wnum : byte; wert : word);
  function form_getslider(wnum : byte) : word;
  procedure form_setslidermax(wnum : byte; max : word);
  function dhslider(x,y,brt : byte; wert,max : word) : string;
  function dvslider(x,y,hhe : byte; wert,max : word) : string;
  function _hslider : integer;
  function _vslider : integer;
  function stn(var strng : string; ch : char) : integer;
  function fileselect(title,path,exts,default : string) : string;


implementation

  const fmax = 511;

  var was,has,saw,haw,fox,foy : byte;
      dalphanum,oxp,oyp,q     : byte;
      oanz,fanf,fanz,exanz    : integer;
      hslnum,vslnum,lnum      : integer;
      files                   : array [0..fmax] of string[12];
      pfad,dummy              : string;
      extension               : array [0..4] of string[12];
      sldelay,slmdelay        : word;


  {$I c:\pascal\units\asv_inc\asv_oinc.inc}


procedure get_files(path : string);
  var anz   : integer;
      srec  : searchrec;
      dummy : string[12];
      nvrnd : boolean;

  begin
    oanz:=-1;
    fanf:=-1;
    fanz:=-1;
    findfirst(path+'*.*',directory,srec);
    while (doserror=0) and (oanz<fmax) do
      begin
        if ((srec.attr and directory)>0) and (srec.name<>'.') then
          begin
            inc(oanz);
            files[oanz]:=srec.name;
          end;
        findnext(srec);
      end;
    anz:=oanz;
    fanf:=succ(oanz);
    for q:=0 to exanz do
      begin
        findfirst(path+extension[q],readonly+archive,srec);
        while (doserror=0) and (anz<fmax) do
          begin
            inc(anz);
            inc(fanz);
            files[anz]:=srec.name;
            findnext(srec);
          end;
      end;
    if fanz>9 then form_setslidermax(15,fanz-9)
      else form_setslidermax(15,0);
    if oanz>4 then form_setslidermax(16,oanz-4)
      else form_setslidermax(16,0);
    form_setslider(15,0);
    form_setslider(16,0);
    if oanz>0 then
      begin
        repeat
          nvrnd:=true;
          for anz:=oanz downto 1 do
            if files[anz]<files[pred(anz)] then
              begin
                dummy:=files[anz];
                files[anz]:=files[pred(anz)];
                files[pred(anz)]:=dummy;
                nvrnd:=false;
              end;
        until nvrnd;
      end;
    if fanz>0 then
      begin
        repeat
          nvrnd:=true;
          for anz:=fanf+fanz downto succ(fanf) do
            if files[anz]<files[pred(anz)] then
              begin
                dummy:=files[anz];
                files[anz]:=files[pred(anz)];
                files[pred(anz)]:=dummy;
                nvrnd:=false;
              end;
        until nvrnd;
      end;
  end;


procedure fl_invert(x,y : byte);

  begin
    setattr(succ(x),succ(y+lnum),was,14);
    form_setan(8,files[fanf+form_getslider(15)+lnum]);
    byteout[2]:=oxp;
    byteout[3]:=oyp;
    form_objcall(dalphanum,8,0,0,0);
  end;


procedure list_print(wnum,x,y,h : byte);
  var wert,max : word;
      anz,anf  : integer;
      bis,sb   : byte;
      name     : string[16];

  begin
    textattr:=saw;
    inc(x);
    inc(y);
    if wnum=16 then
      begin
        anz:=oanz;
        anf:=0;
        sb:=16;
      end
    else
      begin
        anz:=fanz;
        anf:=fanf;
        sb:=14;
      end;
    if anz=-1 then
      begin
        if wnum=15 then for q:=y to pred(y+h) do printxy(x,q,'              ')
          else for q:=y to pred(y+h) do printxy(x,q,'                ');
        exit;
      end;
    wert:=form_getslider(wnum);
    max:=form_getslidermax(wnum);
    if max=0 then bis:=anz else bis:=pred(h);
    for q:=0 to bis do
      begin
        name:=' '+files[anf+wert+q];
        name:=name+space(sb-length(name));
        printxy(x,y+q,name);
      end;
    if bis<pred(h) then
      begin
        if wnum=15 then for q:=succ(bis) to pred(h) do printxy(x,y+q,'              ')
          else for q:=succ(bis) to pred(h) do printxy(x,y+q,'                ');
      end;
    if lnum>-1 then
      begin
        if wnum=15 then fl_invert(pred(x),pred(y))
          else setattr(x,y+lnum,was,16);
      end;
  end;


procedure liste(wnum,com,dat1,dat2 : byte);
  var dtxta,x,y,wn6,bp,ly : byte;
      ow,om               : word;
      hkey                : char;
      entryok             : boolean;

  begin
    dtxta:=textattr;
    oxp:=byteout[2];
    oyp:=byteout[3];
    x:=oxp+ord(work[wnum,3]);
    y:=oyp+ord(work[wnum,4]);
    hkey:=work[wnum,9];
    textattr:=saw;
    wn6:=wnum+6;
    case com of
      0: begin
           verslider(wn6,0,0,0);
           textattr:=haw;
           printxy(byteout[2]+ord(work[wnum,7]),byteout[3]+ord(work[wnum,8]),hkey);
           list_print(wn6,x,y,ord(work[wnum,6])-2);
         end;
      1: if ((wnum=9) and (fanz>=0)) or ((wnum=10) and (oanz>=0)) then
           begin
             byteout[0]:=succ(ord(work[wnum,3]));
             byteout[1]:=succ(ord(work[wnum,4]));
             byteout[2]:=byteout[0]+ord(work[wnum,5])-2;
             byteout[3]:=byteout[1]+ord(work[wnum,6])-3;
             byteout[4]:=0;
           end
         else for ly:=0 to 4 do byteout[ly]:=0;
      2: begin
           if dat1>0 then
             begin
               if (dat1=pred(x+ord(work[wnum,5]))) and (dat2>y) and (dat2<pred(y+ord(work[wnum,6]))) then
                 begin
                   lnum:=0;
                   if wnum=9 then fl_invert(x,y)
                     else setattr(succ(x),succ(y),was,16);
                   ow:=form_getslider(wn6);
                   verslider(wn6,2,dat1,dat2);
                   dec(lnum,form_getslider(wn6)-ow);
                   if lnum<0 then lnum:=0;
                   if wnum=9 then
                     begin
                       if lnum>9 then lnum:=9;
                     end
                   else
                     begin
                       if lnum>4 then lnum:=4;
                     end;
                   list_print(wn6,x,y,ord(work[wnum,6])-2);
                 end
               else
                 begin
                   lnum:=pred(dat2-y);
                   if wnum=9 then
                     begin
                       if (form_getslidermax(wn6)=0) and (lnum>fanz) then lnum:=fanz;
                       fl_invert(x,y);
                     end
                   else
                     begin
                       if (form_getslidermax(wn6)=0) and (lnum>oanz) then lnum:=oanz;
                       setattr(succ(x),succ(lnum)+y,was,16);
                     end;
                   repeat until (mouseb=0) or (mousey<>dat2);
                 end;
             end
           else
             begin
               lnum:=0;
               if wnum=9 then fl_invert(x,y)
                 else setattr(succ(x),succ(y),was,16);
             end;
           byteout[3]:=0;
           hide;
         end;
      3: if ((wnum=9) and (fanz>=0)) or ((wnum=10) and (oanz>=0)) then
           begin
             lnum:=0;
             if wnum=9 then fl_invert(x,y)
               else setattr(succ(x),succ(y),was,16);
             byteout[3]:=0;
           end
         else
           begin
             if byteout[4]=1 then byteout[3]:=pred(wnum)
               else byteout[3]:=succ(wnum);
           end;
      5: if (upper(hkey)=upper(chr(dat2))) and (((wnum=9) and (fanz>=0)) or ((wnum=10) and (oanz>=0))) then byteout[0]:=1
           else byteout[0]:=0;
      7: begin
           if wnum=9 then setattr(succ(x),succ(y+lnum),saw,14)
             else setattr(succ(x),succ(y+lnum),saw,16);
           lnum:=-1;
         end;
     10: begin
           if dat1=KEY_SPEC then
             case dat2 of
               77,80: begin
                        inc(lnum);
                        if form_getslidermax(wn6)=0 then
                          begin
                            if wnum=9 then
                              begin
                                if lnum>fanz then lnum:=fanz;
                              end
                            else
                              begin
                                if lnum>oanz then lnum:=oanz;
                              end;
                          end
                        else
                          begin
                            if wnum=9 then
                              begin
                                if lnum>9 then
                                  begin
                                    lnum:=9;
                                    if form_getslider(wn6)<form_getslidermax(wn6) then
                                      form_setslider(wn6,succ(form_getslider(wn6)));
                                  end;
                              end
                            else
                              begin
                                if lnum>4 then
                                  begin
                                    lnum:=4;
                                    if form_getslider(wn6)<form_getslidermax(wn6) then
                                      form_setslider(wn6,succ(form_getslider(wn6)));
                                  end;
                              end;
                          end;
                        verslider(wn6,0,0,0);
                        list_print(wn6,x,y,ord(work[wnum,6])-2);
                        byteout[3]:=0;
                      end;
               72,75: if (lnum>0) or (form_getslider(wn6)>0) then
                        begin
                          dec(lnum);
                          if lnum<0 then
                            begin
                              lnum:=0;
                              if form_getslider(wn6)>0 then
                                form_setslider(wn6,pred(form_getslider(wn6)));
                            end;
                          verslider(wn6,0,0,0);
                          list_print(wn6,x,y,ord(work[wnum,6])-2);
                          byteout[3]:=0;
                        end;
               73: if form_getslider(wn6)>0 then
                     begin
                       if wnum=9 then
                         begin
                           if form_getslider(wn6)>10 then form_setslider(wn6,form_getslider(wn6)-10)
                             else form_setslider(wn6,0);
                         end
                       else
                         begin
                           if form_getslider(wn6)>5 then form_setslider(wn6,form_getslider(wn6)-5)
                             else form_setslider(wn6,0);
                         end;
                       lnum:=0;
                       verslider(wn6,0,0,0);
                       list_print(wn6,x,y,ord(work[wnum,6])-2);
                       byteout[3]:=0;
                     end;
               81: if form_getslider(wn6)<form_getslidermax(wn6) then
                     begin
                       if wnum=9 then form_setslider(wn6,form_getslider(wn6)+10)
                         else form_setslider(wn6,form_getslider(wn6)+5);
                       if form_getslider(wn6)>form_getslidermax(wn6) then
                         form_setslider(wn6,form_getslidermax(wn6));
                       lnum:=0;
                       verslider(wn6,0,0,0);
                       list_print(wn6,x,y,ord(work[wnum,6])-2);
                       byteout[3]:=0;
                     end;
               71: if (lnum>0) or (form_getslider(wn6)>0) then
                     begin
                       lnum:=0;
                       form_setslider(wn6,0);
                       verslider(wn6,0,0,0);
                       list_print(wn6,x,y,ord(work[wnum,6])-2);
                       byteout[3]:=0;
                     end;
               79: if ((wnum=9) and (lnum<9)) or ((wnum=10) and (lnum<4)) or
                      (form_getslider(wn6)<form_getslidermax(wn6)) then
                     begin
                       if wnum=9 then
                         begin
                           lnum:=9;
                           if (form_getslidermax(wn6)=0) and (lnum>fanz) then lnum:=fanz;
                         end
                       else
                         begin
                           lnum:=4;
                           if (form_getslidermax(wn6)=0) and (lnum>oanz) then lnum:=oanz;
                         end;
                       form_setslider(wn6,form_getslidermax(wn6));
                       verslider(wn6,0,0,0);
                       list_print(wn6,x,y,ord(work[wnum,6])-2);
                       byteout[3]:=0;
                     end;
             else byteout[3]:=0
             end
           else
             if dat2=32 then
               begin
                 if wnum=9 then
                   begin
                     intout[0]:=0;
                     byteout[3]:=254;
                   end
                 else
                   begin
                     if files[form_getslider(16)+lnum]='..' then
                       begin
                         cutr(pfad);
                         for q:=1 to length(pfad) do if pfad[q]='\' then bp:=q;
                         pfad:=left(pfad,bp);
                       end
                     else pfad:=pfad+files[form_getslider(16)+lnum]+'\';
                     dummy:=pfad;
                     for q:=0 to exanz do dummy:=dummy+extension[q]+',';
                     cutr(dummy);
                     dummy:=right(dummy,43);
                     printxy(oxp+2,oyp+3,dummy+space(43-length(dummy)));
                     get_files(pfad);
                     verslider(15,0,0,0);
                     verslider(16,0,0,0);
                     lnum:=-1;
                     list_print(15,oxp+2,oyp+5,10);
                     lnum:=0;
                     list_print(16,oxp+22,oyp+10,5);
                     byteout[3]:=0;
                   end;
               end
             else byteout[3]:=0;
         end;
     11: begin
           if (dat1=pred(x+ord(work[wnum,5]))) and (dat2>y) and (dat2<pred(y+ord(work[wnum,6]))) then
             begin
               ow:=form_getslider(wn6);
               verslider(wn6,11,dat1,dat2);
               dec(lnum,form_getslider(wn6)-ow);
               if lnum<0 then lnum:=0;
               if wnum=9 then
                 begin
                   if lnum>9 then lnum:=9;
                 end
               else
                 begin
                   if lnum>4 then lnum:=4;
                 end;
               list_print(wn6,x,y,ord(work[wnum,6])-2);
               byteout[3]:=0;
             end
           else
             begin
               if lnum<>pred(dat2-y) then
                 begin
                   entryok:=true;
                   if wnum=9 then
                     begin
                       if (form_getslidermax(wn6)=0) and (pred(dat2-y)>fanz) then
                         begin
                           repeat until (mouseb=0) or ((mousey<>succ(lnum)+y) and (mousey<>dat2));
                           entryok:=false;
                         end;
                     end
                   else
                     begin
                       if (form_getslidermax(wn6)=0) and (pred(dat2-y)>oanz) then
                         begin
                           repeat until (mouseb=0) or ((mousey<>succ(lnum)+y) and (mousey<>dat2));
                           entryok:=false;
                         end;
                     end;
                   if entryok then
                     begin
                       if wnum=9 then
                         begin
                           setattr(succ(x),succ(y+lnum),saw,14);
                           lnum:=pred(dat2-y);
                           fl_invert(x,y);
                         end
                       else
                         begin
                           setattr(succ(x),succ(y+lnum),saw,16);
                           lnum:=pred(dat2-y);
                           setattr(succ(x),dat2,was,16);
                         end;
                       repeat
                         ly:=mousey;
                         if ly>=pred(y+ord(work[wnum,6])) then
                           begin
                             ow:=form_getslider(wn6);
                             om:=form_getslidermax(wn6);
                             while (ow<om) and (mouseb>0) and (mousey>=pred(y+ord(work[wnum,6]))) do
                               begin
                                 form_setslider(wn6,succ(ow));
                                 if wnum=9 then
                                   begin
                                     if lnum<9 then inc(lnum);
                                   end
                                 else
                                   begin
                                     if lnum<4 then inc(lnum);
                                   end;
                                 verslider(wn6,0,0,0);
                                 list_print(wn6,x,y,ord(work[wnum,6])-2);
                                 delay(sldelay);
                                 ow:=form_getslider(wn6);
                               end;
                             repeat until (mouseb=0) or (mousey<y+ord(work[wnum,6])-2);
                           end;
                         if ly<=y then
                           begin
                             ow:=form_getslider(wn6);
                             while (ow>0) and (mouseb>0) and (mousey<=y) do
                               begin
                                 form_setslider(wn6,pred(ow));
                                 if lnum>0 then dec(lnum);
                                 verslider(wn6,0,0,0);
                                 list_print(wn6,x,y,ord(work[wnum,6])-2);
                                 delay(sldelay);
                                 ow:=form_getslider(wn6);
                               end;
                             repeat until (mouseb=0) or (mousey>succ(y));
                           end;
                       until (mouseb=0) or (ly<>dat2);
                     end;
                   byteout[3]:=0;
                 end
               else
                 begin
                   if wnum=9 then
                     begin
                       intout[0]:=0;
                       byteout[3]:=254;
                     end
                   else
                     begin
                       if files[form_getslider(16)+lnum]='..' then
                         begin
                           cutr(pfad);
                           for q:=1 to length(pfad) do if pfad[q]='\' then bp:=q;
                           pfad:=left(pfad,bp);
                         end
                       else pfad:=pfad+files[form_getslider(16)+lnum]+'\';
                       dummy:=pfad;
                       for q:=0 to exanz do dummy:=dummy+extension[q]+',';
                       cutr(dummy);
                       dummy:=right(dummy,43);
                       printxy(oxp+2,oyp+3,dummy+space(43-length(dummy)));
                       get_files(pfad);
                       verslider(15,0,0,0);
                       verslider(16,0,0,0);
                       lnum:=-1;
                       list_print(15,oxp+2,oyp+5,10);
                       lnum:=0;
                       list_print(16,oxp+22,oyp+10,5);
                       repeat until (mouseb=0) or (mousey<>dat2);
                       byteout[3]:=0;
                     end;
                 end;
             end;
         end;
    end;
    textattr:=dtxta;
  end;


function fileselect(title,path,exts,default : string) : string;
  label fs_exit;

  var but,lnr  : integer;
      worksave : array [0..16] of string[90];

  begin
    lnr:=form_objins(liste);
    if lnr=F_ERROR then goto fs_exit;
    for q:=0 to 16 do worksave[q]:=work[q];
    default:=upper(right(default,12));
    if length(path)=0 then getdir(0,path);
    path:=upper(path);
    if right(path,1)<>'\' then path:=path+'\';
    if (length(path)<3) or (path[2]<>':') then
      begin
        regs.ah:=$19;
        intr($21,regs);
        path:=chr(regs.al+65)+':'+path;
      end;
    if path[3]<>'\' then path:=left(path,2)+'\'+right(path,length(path)-2);
    exanz:=-1;
    for q:=0 to 4 do extension[q]:='';
    dummy:=upper(exts);
    while (pos(',',dummy)>0) and (exanz<4) do
      begin
        q:=pos(',',dummy);
        pfad:=trimf(left(dummy,pred(q)));
        if length(pfad)>0 then
          begin
            inc(exanz);
            extension[exanz]:=pfad;
          end;
        dummy:=right(dummy,length(dummy)-q);
      end;
    trim(dummy);
    if (length(dummy)>0) and (exanz<4) then
      begin
        inc(exanz);
        extension[exanz]:=dummy;
      end;
    if exanz=-1 then
      begin
        exanz:=0;
        extension[0]:='*.*';
      end;
    pfad:=path;
    work[0]:=chr(fox)+chr(foy)+#0#0#47#20+chr(byteout[4]);
    work[1]:=left(title,35);
    work[2]:=dtext(2,2,'Dateien in:');
    dummy:=path;
    for q:=0 to exanz do dummy:=dummy+extension[q]+',';
    cutr(dummy);
    work[3]:=dtext(2,3,right(dummy,43));
    work[4]:=dframe(2,5,16,12,'');
    work[5]:=dframe(22,10,18,7,'');
    work[6]:=dtext(23,9,'Verzeichnisse:');
    work[7]:='';
    work[8]:=dalpha(19,6,13,12,B_ENABLED,'Datei#name:',default);
    work[9]:=#255+chr(lnr)+#2#5#16#12#2#2'D';
    work[10]:=#255+chr(lnr)+#22#10#18#7#23#9'V';
    work[11]:='  #OK  ';
    work[12]:='#Abbruch';
    work[13]:='';
    work[14]:='';
    work[15]:=dvslider(17,6,10,0,0);
    work[16]:=dvslider(39,11,5,0,0);
    get_files(path);
    lnum:=-1;
    but:=dialog;
    fox:=byteout[0];  foy:=byteout[1];
    dummy:=trimf(form_getan(8));
    if (but=0) and (length(dummy)>0) then fileselect:=pfad+dummy
      else fileselect:='';
    for q:=0 to 16 do work[q]:=worksave[q];
    void.i:=form_objdel(lnr);
    fs_exit:
  end;


begin
  vslnum:=form_objins(verslider);
  hslnum:=form_objins(horslider);
  menu_inquire;
  dalphanum:=byteout[24];
  obj_setcolor(byteout[0],byteout[2],byteout[15],byteout[16]);
  form_setslsdelay(75);
  form_setslmdelay(150);
  fox:=255;  foy:=255;
end.