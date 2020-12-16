{ ASV 1.4 MENU vom 13.06.1991 }

procedure form_seterrl(cc : integer);

  begin
    if (cc=L_USA) or (cc=L_FRG) or (cc=L_GB) then ccode:=cc;
  end;


procedure form_setattr(bits : word);

  begin
    dbits:=(bits and 7);
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

  begin
    asvierror:=0;
    if m_stat=-1 then
      begin
        for wq:=0 to _wmax do
          begin
            mwork[wq]:=left(work[wq],30);
            attrib[wq]:=0;
          end;
        wq:=1;
        get_hotkey(0);
        m_max:=0;
        mbegin[0]:=0;
        mxpos[0]:=3;
        mxpos[1]:=4+length(mwork[0]);
        repeat
          get_hotkey(wq);
          if length(mwork[wq])=0 then
            begin
              if length(mwork[succ(wq)])<>0 then
                begin
                  inc(wq);
                  inc(m_max);
                  mbegin[m_max]:=wq;
                  get_hotkey(wq);
                  mxpos[succ(m_max)]:=succ(mxpos[m_max])+length(mwork[wq]);
                end;
            end;
          inc(wq);
        until (wq=pred(_wmax)) or ((length(mwork[wq])=0) and (length(mwork[succ(wq)])=0));
        max_entry:=wq;
        for wq:=0 to _mmax do mecurr[wq]:=1;
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
  var dummy,owx,owy : byte;

  begin
    asvierror:=0;
    if (m_stat=0) or (m_stat=1) then
      begin
        dummy:=textattr;
        owx:=wherex;
        owy:=wherey;
        hidemouse;
        if m_stat=0 then
          for wq:=0 to pred(anzcol) do titleram[wq]:=memw[vioseg:(wq shl 1)];
        textattr:=schwarzaufweiss;
        printxy(1,1,space(anzcol));
        for wq:=0 to m_max do printxy(mxpos[wq],1,mwork[mbegin[wq]]);
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
        mp_dis(mbegin[num],1);
        hide;
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
        if (meny=1) and (menx>1) and (menx<mxpos[succ(m_max)]) and (mpd_stat=M_DROP) then
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
          end
        else mo_cmouse:=0;
      end;
  end;


function menu_open(num : integer) : integer;
  var xc,q,me_anz,vrnum : integer;
      flag,cok,mmret,xl : integer;
      oldattr           : byte;
      taste             : char;

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
            get_vioram(xc,2,xl+2,me_anz+2,0);
            oldattr:=textattr;
            textattr:=schwarzaufweiss;
            printxy(xc,2,'Ú'+strg('Ä',xl)+'¿');
            for q:=1 to me_anz do
              if mwork[mbegin[num]+q]='-' then printxy(xc,q+2,'Ã'+strg('Ä',xl)+'´')
              else
                begin
                  printxy(xc,q+2,'³ '+mwork[mbegin[num]+q]+space(xl-length(mwork[mbegin[num]+q])-2)+' ³');
                  if mhpos[mbegin[num]+q]<>0 then setattr(xc+mhpos[mbegin[num]+q]+1,q+2,hellaufweiss,1);
                  if (attrib[mbegin[num]+q] and A_CHECKED)=A_CHECKED then mem[vioseg:((q+1)*loff+xc*2)]:=sel_char;
                  if (attrib[mbegin[num]+q] and A_DISABLED)=A_DISABLED then setattr(xc+1,q+2,disabled,xl);
                end;
            printxy(xc,3+me_anz,'À'+strg('Ä',xl)+'Ù');
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
                      else if (taste=#13) or (taste=#27) then flag:=M_CANCEL;
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
                              if upchar(taste)=upchar(mhkey[mbegin[num]+q]) then
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
            put_vioram(xc,2,xl+2,me_anz+2,0);
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
        begin
          if (wy=1) and (wx>1) and (wx<mxpos[succ(m_max)]) then
            begin
              if wx=2 then m_num:=0
                else for q:=0 to m_max do if (wx>=mxpos[q]) and (wx<mxpos[succ(q)]) then m_num:=q;
              if length(mwork[succ(mbegin[m_num])])=0 then flag:=mbegin[m_num]
                else flag:=menu_odo;
            end
          else flag:=M_CANCEL;
        end
      else
        if mpd_stat=M_DROP then
          begin
            if (wy=1) and (wx>1) and (wx<mxpos[succ(m_max)]) then
              begin
                if wx=2 then m_num:=0
                  else for q:=0 to m_max do if (wx>=mxpos[q]) and (wx<mxpos[succ(q)]) then m_num:=q;
                if length(mwork[succ(mbegin[m_num])])=0 then flag:=mbegin[m_num]
                  else flag:=menu_odo;
              end;
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
                    if upchar(taste)=upchar(mhkey[mbegin[q]]) then
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
    mp_dis(M_EXIT,1);
    hide;
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
        if ((wb and M_LEFT)=M_LEFT) or (mpd_stat=M_DROP) then
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
        setattr(wox,woy,saw_pop,len);
        if woh>0 then setattr(wox+woh,woy,haw_pop,1);
      end;
  end;


function menu_pop(wo : byte) : integer;
  label all_dis;

  var flag,plen,plmax,q,def,oms : integer;
      taste                     : char;
      mpnum,owx,owy,xp,yp,dpos  : byte;
      ppos,brt,hhe,cflag,dflag  : byte;
      wx,wy,wb,dtxattr          : byte;
      phkpos,pmattr,spstr       : string;
      dummy                     : string[1];

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
                  dhkstr:=dhkstr+upchar(dummy[1]);
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
        get_vioram(xp,yp,brt,hhe,cflag);
        textattr:=saw_pop;
        if (dbits and D_GRAF)>0 then graf_growbox(xp,yp,brt,hhe);
        printxy(xp,yp,'Ú'+strg('Ä',brt-2)+'¿');
        spstr:='³'+space(brt-2)+'³';
        dflag:=0;
        for q:=succ(yp) to yp+hhe-2 do
          if dwork[succ(wo+q-yp)]='-' then printxy(xp,q,'Ã'+strg('Ä',brt-2)+'´')
          else
            begin
              printxy(xp,q,spstr);
              printxy(xp+2,q,dwork[succ(wo+q-yp)]);
              if (ord(pmattr[q-yp]) and A_CHECKED)=A_CHECKED then mem[vioseg:(pred(q)*loff+xp*2)]:=sel_char;
              if (ord(pmattr[q-yp]) and A_DISABLED)=A_DISABLED then setattr(succ(xp),q,dis_pop,brt-2)
              else
                begin
                  if ord(phkpos[q-yp])>0 then setattr(succ(xp+ord(phkpos[q-yp])),q,haw_pop,1);
                  dflag:=1;
                end;
            end;
        printxy(xp,pred(yp+hhe),'À'+strg('Ä',brt-2)+'Ù');
        if farbe then
          begin
            for q:=succ(yp) to yp+hhe do setattr(xp+brt,q,shadow,1);
            setattr(succ(xp),yp+hhe,shadow,brt);
          end;
        if length(work[succ(wo)])>0 then
          printxy(pred(xp+(brt shr 1)-(length(work[succ(wo)]) shr 1)),yp,' '+work[succ(wo)]+' ');
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
            until (taste=#13) or (taste=#27);
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
                    dpos:=pos(upchar(taste),dhkstr);
                    if (dpos>0) and (dpos<=(hhe-2)) then
                      if ord(pmattr[dpos])<>A_DISABLED then flag:=pred(dpos);
                  end;
                end;
            end;
        until flag<>M_NOP;
        hidemouse;
        mp_dis(M_EXIT,oms);
        all_dis:
        put_vioram(xp,yp,brt,hhe,cflag);
        textattr:=dtxattr;
        gotoxy(owx,owy);
        repeat until mouseb=0;
        showmouse;
        menu_pop:=flag;
      end
    else menu_pop:=E_NREENT;
  end;


procedure form_extcol(sdia,hdia,sale,hale,spop,hpop,dpop : byte);

  begin
    if m_stat<>2 then
      begin
        saw_dia:=sdia;
        haw_dia:=hdia;
        saw_ale:=sale;
        haw_ale:=hale;
        saw_pop:=spop;
        haw_pop:=hpop;
        dis_pop:=dpop;
      end;
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
        form_extcol(saw,haw,saw,haw,saw,haw,dis);
      end;
  end;


procedure menu_resetc;

  begin
    if farbe then
      begin
        menu_setc(attr(lightgray,black),attr(black,lightgray),attr(green,black),
                  attr(red,lightgray),attr(darkgray,lightgray),attr(darkgray,black));
        form_extcol(attr(yellow,cyan),attr(blue,cyan),attr(yellow,red),attr(black,red),
                    attr(black,lightgray),attr(red,lightgray),attr(darkgray,lightgray));
      end
    else menu_setc(7,112,15,127,127,0);
  end;


procedure menu_setschr(ccode : byte);

  begin
    if (ccode<>0) and (ccode<>32) and (ccode<>255) then sel_char:=ccode;
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
  end;