{ ASV 1.5 DIAL vom 18.12.1991 }

procedure but_invert(def,bdw,bypos,was,wie : byte);
  var dummy : byte;

  begin
    dummy:=textattr;
    textattr:=was;
    if wie=1 then
      begin
        printxy(bxpos[def],bypos,'®');
        printxy(succ(bxpos[def]+length(dwork[bdw+def])),bypos,'¯');
      end
    else
      begin
        printxy(bxpos[def],bypos,'<');
        printxy(succ(bxpos[def]+length(dwork[bdw+def])),bypos,'>');
      end;
    textattr:=dummy;
    crsof:=1;
    while (dwork[bdw+def,crsof]=' ') and (crsof<length(dwork[bdw+def])) do inc(crsof);
    gotoxy(bxpos[def]+crsof,bypos);
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
        dhkstr:=upchar(dummy[1])+dhkstr;
        delete(dwork[num],hpos,1);
      end
    else dhkstr:=#0+dhkstr;
  end;


procedure mp_dis(msg : byte; mval : integer);

  begin
    m_stat:=2;
    m_pcall(msg);
    m_stat:=mval;
  end;


function move_box(xp,yp,xpn,ypn,hhe,brt,cflag,banz,def,sf : byte; var xpv,ypv : byte; var bpv : integer) : boolean;
  var vrnum,xdif,ydif,ym,xm,vno : integer;
      xvon,xbis,yvon,ybis,vdif  : integer;
      h,b,xv,xb,yv,yb           : byte;
      q,vofs,vofs2,vofst        : word;

  begin
    if p1<>nil then
      begin
        hidemouse;
        xdif:=xpn-xp;
        ydif:=ypn-yp;
        p2:=p1;
        vofs2:=pred(ypn)*loff+pred(xpn)*2;
        q:=vofs2;
        for h:=0 to hhe-cflag do
          begin
            vofs:=q;
            for b:=0 to brt-cflag do
              begin
                p2^:=memw[vioseg:vofs];
                inc(vofs,2);
                inc(longint(p2),2);
              end;
            inc(q,loff);
          end;
        vofst:=pred(yp)*loff+pred(xp)*2;
        vdif:=vofst-vofs2;
        if ydif>0 then
          begin
            ym:=-1;
            inc(vofst,pred(hhe)*loff);
          end
        else ym:=1;
        if xdif>0 then
          begin
            xm:=-1;
            inc(vofst,pred(brt)*2);
          end
        else xm:=1;
        xvon:=xm*2;
        yvon:=ym*loff;
        for h:=0 to pred(hhe) do
          begin
            vofs:=vofst;
            vofs2:=vofs-vdif;
            for b:=0 to pred(brt) do
              begin
                memw[vioseg:vofs2]:=memw[vioseg:vofs];
                inc(vofs,xvon);
                inc(vofs2,xvon);
              end;
            inc(vofst,yvon);
          end;
        vdif:=succ(brt)-cflag;
        if (abs(xdif)+cflag>brt) or (abs(ydif)+cflag>hhe) then put_vioram(xp,yp,brt,hhe,cflag)
        else
          begin
            if xdif<0 then
              begin
                xvon:=xp+brt+xdif;
                xbis:=xp+brt-cflag;
                xv:=xp;
                xb:=xvon-cflag;
              end
            else
              begin
                xvon:=xp;
                xbis:=xp+pred(xdif);
                xv:=succ(xbis);
                xb:=xp+brt-cflag;
              end;
            if ydif<0 then
              begin
                yvon:=yp+hhe+ydif;
                ybis:=yp+hhe-cflag;
                yv:=yp;
                yb:=yvon-cflag;
              end
            else
              begin
                yvon:=yp;
                ybis:=yp+pred(ydif);
                yv:=succ(ybis);
                yb:=yp+hhe-cflag;
              end;
            if xbis>=xvon then
              begin
                q:=pred(yp)*loff+pred(xvon)*2;
                vno:=xvon-xp;
                for h:=0 to hhe-cflag do
                  begin
                    vofs:=q;
                    vrnum:=vno;
                    for b:=xvon to xbis do
                      begin
                        memw[vioseg:vofs]:=vioram[vrnum];
                        inc(vofs,2);
                        inc(vrnum);
                      end;
                    inc(q,loff);
                    inc(vno,vdif);
                  end;
              end
            else
              begin
                xv:=xp;
                xb:=xp+brt-cflag;
              end;
            if ybis>=yvon then
              begin
                q:=pred(yvon)*loff+pred(xp)*2;
                vrnum:=(yvon-yp)*vdif;
                for h:=yvon to ybis do
                  begin
                    vofs:=q;
                    for b:=0 to brt-cflag do
                      begin
                        memw[vioseg:vofs]:=vioram[vrnum];
                        inc(vofs,2);
                        inc(vrnum);
                      end;
                    inc(q,loff);
                  end;
              end
            else
              begin
                yv:=yp;
                yb:=yp+hhe-cflag;
              end;
            vno:=(yv-yp)*vdif+xv-xp;
            p2:=p1;
            if ydif<0 then dec(longint(p2),ydif*vdif*2);
            if xdif<0 then dec(longint(p2),xdif*2);
            xbis:=vdif*2;
            for h:=yv to yb do
              begin
                p3:=p2;
                vrnum:=vno;
                for b:=xv to xb do
                  begin
                    p3^:=vioram[vrnum];
                    inc(vrnum);
                    inc(longint(p3),2);
                  end;
                inc(vno,vdif);
                inc(longint(p2),xbis);
              end;
          end;
        vrnum:=0;
        p2:=p1;
        for h:=0 to hhe-cflag do
          for b:=0 to brt-cflag do
            begin
              vioram[vrnum]:=p2^;
              inc(vrnum);
              inc(longint(p2),2);
            end;
        if (farbe) and (sf=1) then
          begin
            memw[vioseg:(pred(ypn)*loff+pred(xpn+brt)*2)]:=vioram[brt];
            memw[vioseg:(pred(ypn+hhe)*loff+pred(xpn)*2)]:=vioram[succ(brt)*hhe];
            for q:=ypn+1 to ypn+hhe do setattr(xpn+brt,q,shadow,1);
            setattr(succ(xpn),ypn+hhe,shadow,brt);
          end;
        inc(xpv,xdif);
        inc(ypv,ydif);
        inc(bpv,ydif);
        if (wherex=anzcol) and (wherey=anzline) then hide
          else gotoxy(wherex+xdif,wherey+ydif);
        for q:=0 to banz do inc(bxpos[q],xdif);
        showmouse;
        move_box:=true;
      end
    else move_box:=false;
  end;


procedure obj_call(wnum,com,dat1,dat2,xp,yp : byte);
  var denum : byte;

  begin
    denum:=ord(dwork[wnum,2]);
    if denum<=_dobjmax then
      if @dobjmem[denum]<>@dobj_nothing then
        begin
          byteout[2]:=xp;
          byteout[3]:=yp;
          dobjmem[denum](wnum,com,dat1,dat2);
        end;
  end;


procedure get_activate(aktiv,xp,yp : byte; var mix,miy,max,may,ke : byte);

  begin
    obj_call(aktiv,1,0,0,xp,yp);
    mix:=byteout[0];
    miy:=byteout[1];
    max:=byteout[2];
    may:=byteout[3];
    ke:=byteout[4];
  end;


procedure obj_activate(aktiv,xp,yp,was : byte; var mix,miy,max,may,ke : byte);

  begin
    get_activate(aktiv,xp,yp,mix,miy,max,may,ke);
    byteout[4]:=was;
    obj_call(aktiv,3,0,0,xp,yp);
  end;


procedure new_active(bdw,banz,bypos,xp,yp,dv : byte; var aktiv,mix,miy,max,may,ke : byte; var def,flag : integer);
  label newac_again;

  var nac,wohin : byte;

  begin
    newac_again:
    nac:=byteout[3];
    wohin:=byteout[4];
    if nac=255 then nac:=bdw+banz;
    if nac=254 then flag:=intout[0]
    else
      begin
        if (nac>=dv) and (nac<=bdw+banz) then
          begin
            if aktiv>=bdw then
              begin
                if nac<bdw then but_invert(def,bdw,bypos,schwarzaufweiss,1);
              end
            else obj_call(aktiv,7,0,0,xp,yp);
            aktiv:=nac;
            if aktiv>=bdw then
              begin
                but_invert(def,bdw,bypos,schwarzaufweiss,0);
                def:=aktiv-bdw;
                but_invert(def,bdw,bypos,hellaufweiss,1);
              end
            else
              begin
                obj_activate(aktiv,xp,yp,wohin,mix,miy,max,may,ke);
                goto newac_again;
              end;
          end;
      end;
  end;


procedure graf_growbox(x,y,b,h : byte);
  var xp,yp,q,nxp,nyp,w : byte;
      dummy,dummy2      : string;

  begin
    xp:=x+(b shr 1);
    yp:=y+(h shr 1);
    dummy:='';
    for q:=0 to b-2 do
      begin
        nxp:=pred(xp-(q shr 1));
        printxy(nxp,yp,'Ú'+dummy+'¿');
        printxy(nxp,succ(yp),'À'+dummy+'Ù');
        dummy:=dummy+'Ä';
        delay(1);
      end;
    cutr(dummy);
    dummy2:='³'+space(b-2)+'³';
    for q:=1 to h-2 do
      begin
        nyp:=pred(yp-(q shr 1));
        printxy(nxp,nyp,'Ú'+dummy+'¿');
        for w:=1 to q do printxy(nxp,nyp+w,dummy2);
        printxy(nxp,succ(nyp+q),'À'+dummy+'Ù');
        delay(1);
      end;
  end;


function dialog_do : integer;
  label derrjmp;

  var xp,yp,brt,hhe,dummy,owx,owy,fh,fy  : byte;
      dpos,oms,wx,wy,wb,xflag,fb,dn,dtx  : byte;
      wxo,wyo,wdif,fyc,cflag,devon,debis : byte;
      minx,maxx,miny,maxy,keyev,hx,hy    : byte;
      taste                              : char;
      q,banz,bdw,def,bypos,flag,l1,xd,yd : integer;
      spstr                              : string[200];
      destat                             : boolean;

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
        if (xp+brt-cflag>anzcol) or (yp+hhe-cflag>anzline) or (brt<2) or (hhe<3) then
          begin
            dialog_do:=E_MOUTSCR;
            exit;
          end;
        hidemouse;
        get_vioram(xp,yp,brt,hhe,cflag);
        textattr:=schwarzaufweiss;
        if (dbits and D_GRAF)>0 then graf_growbox(xp,yp,brt,hhe);
        if (dbits and D_MOVE)>0 then printxy(xp,yp,'Õ'+strg('Í',brt-2)+'¸')
          else printxy(xp,yp,'Ú'+strg('Ä',brt-2)+'¿');
        spstr:='³'+space(brt-2)+'³';
        bypos:=yp+hhe-2;
        for q:=yp+1 to bypos do printxy(xp,q,spstr);
        printxy(xp,succ(bypos),'À'+strg('Ä',brt-2)+'Ù');
        if farbe then
          begin
            for q:=yp+1 to yp+hhe do setattr(xp+brt,q,shadow,1);
            setattr(succ(xp),yp+hhe,shadow,brt);
          end;
        if length(dwork[1])>0 then printxy(pred(xp+(brt shr 1)-(length(dwork[1]) shr 1)),yp,' '+dwork[1]+' ');
        if (dbits and D_CLOSE)>0 then printxy(xp+2,yp,'[þ]');
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
                            printxy(xflag,fy,'Ú'+strg('Ä',fb-2)+'¿');
                            printxy(xflag,pred(fy+fh),'À'+strg('Ä',fb-2)+'Ù');
                            if (length(dwork[q])>6) and (fb>4) then
                              begin
                                wdif:=xflag+(fb shr 1)-((length(dwork[q])-4) shr 1);
                                if ((fb and 254)=fb) and ((length(dwork[q]) and 254)<>length(dwork[q])) then dec(wdif);
                                printxy(wdif,fy,' '+right(dwork[q],length(dwork[q])-6)+' ');
                              end;
                            if fh>2 then
                              for fyc:=0 to fh-3 do
                                begin
                                  inc(fy);
                                  printxy(xflag,fy,'³');
                                  printxy(pred(xflag+fb),fy,'³');
                                end;
                          end;
                      end
                    else printxy(xp+xflag,yp+ord(dwork[q,2]),right(dwork[q],length(dwork[q])-2));
                  end;
              end;
            inc(q);
          end;
        inc(q);
        if (length(dwork[q])>1) and (dwork[q,1]=#255) then
          begin
            destat:=true;
            devon:=q;
            while (length(dwork[q])>1) and (dwork[q,1]=#255) do inc(q);
            debis:=pred(q);
            for bdw:=0 to _dobjmax do dobjmem[bdw](devon,99,devon,debis);
          end
        else destat:=false;
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
            printxy(bxpos[q],bypos,'<'+dwork[bdw+q]+'>');
            if dhkpos>0 then setattr(bxpos[q]+dhkpos,bypos,hellaufweiss,1);
          end;
        if hhe>3 then printxy(xp,pred(bypos),'Ã'+strg('Ä',brt-2)+'´');
        def:=ord(dwork[0,3]);
        if def>banz then def:=0;
        flag:=M_NOP;
        if destat then
          begin
            aktiv:=devon;
            for q:=devon to debis do obj_call(q,0,0,0,xp,yp);
            but_invert(def,bdw,bypos,schwarzaufweiss,1);
            obj_activate(aktiv,xp,yp,0,minx,miny,maxx,maxy,keyev);
            new_active(bdw,banz,bypos,xp,yp,devon,aktiv,minx,miny,maxx,maxy,keyev,def,flag);
          end
        else
          begin
            aktiv:=bdw+def;
            but_invert(def,bdw,bypos,hellaufweiss,1);
          end;
        textattr:=dummy;
        l1:=succ(hhe*brt) shl 1;
        if maxavail>l1 then getmem(p1,l1) else p1:=nil;
        showmouse;
        repeat
          getmouse(wx,wy,wb);
          if (wb and M_LEFT)=M_LEFT then
            begin
              if wy=bypos then
                begin
                  for q:=0 to banz do
                    if (wx>=bxpos[q]) and (wx<=succ(bxpos[q]+length(dwork[bdw+q]))) then flag:=q;
                end
              else
                if wy=yp then
                  begin
                    if (wx>xp+1) and (wx<xp+5) and ((dbits and D_CLOSE)>0) then flag:=M_CANCEL
                    else
                      if (wx>=xp) and (wx<xp+brt) and ((dbits and D_MOVE)>0) then
                        begin
                          wxo:=wx;
                          wyo:=wy;
                          wdif:=wx-xp;
                          hidemouse;
                          if farbe then
                            begin
                              vofs:=yp*loff+pred(xp+brt)*2;
                              q:=succ(brt*2);
                              for dtx:=1 to hhe do
                                begin
                                  memw[vioseg:vofs]:=vioram[q];
                                  inc(vofs,loff);
                                  inc(q,succ(brt));
                                end;
                              vofs:=pred(yp+hhe)*loff+xp*2;
                              q:=succ(succ(brt)*hhe);
                              for dtx:=2 to brt do
                                begin
                                  memw[vioseg:vofs]:=vioram[q];
                                  inc(vofs,2);
                                  inc(q);
                                end;
                            end;
                          dtx:=textattr;
                          textattr:=schwarzaufweiss;
                          printxy(xp,yp,'É');
                          printxy(pred(xp+brt),yp,'»');
                          for q:=succ(yp) to bypos do
                            begin
                              printxy(xp,q,'º');
                              printxy(pred(xp+brt),q,'º');
                            end;
                          printxy(xp,succ(bypos),'È'+strg('Í',brt-2)+'¼');
                          if hhe>3 then printxy(xp,pred(bypos),'Ç'+strg('Ä',brt-2)+'¶');
                          showmouse;
                          repeat
                            getmouse(wx,wy,wb);
                            if wx-wdif<1 then wx:=succ(wdif);
                            if wx+brt-wdif-cflag>anzcol then wx:=anzcol+wdif+cflag-brt;
                            if wy+hhe-cflag>anzline then wy:=anzline+cflag-hhe;
                            if (wx<>wxo) or (wy<>wyo) then
                              begin
                                void.bo:=move_box(wxo-wdif,wyo,wx-wdif,wy,hhe,brt,cflag,banz,def,0,xp,yp,bypos);
                                wxo:=wx;
                                wyo:=wy;
                              end;
                            with JRECT do
                              begin
                                xl:=xp;
                                yo:=yp;
                                xr:=xp+brt;
                                yu:=yp+hhe;
                                if not(farbe) then
                                  begin
                                    dec(xr);
                                    dec(yu);
                                  end;
                              end;
                            menu_event(S_DIALOG);
                          until wb=0;
                          hidemouse;
                          printxy(xp,yp,'Õ');
                          printxy(pred(xp+brt),yp,'¸');
                          for q:=succ(yp) to bypos do
                            begin
                              printxy(xp,q,'³');
                              printxy(pred(xp+brt),q,'³');
                            end;
                          printxy(xp,succ(bypos),'À'+strg('Ä',brt-2)+'Ù');
                          if hhe>3 then printxy(xp,pred(bypos),'Ã'+strg('Ä',brt-2)+'´');
                          if farbe then
                            begin
                              for q:=yp+1 to yp+hhe do setattr(xp+brt,q,shadow,1);
                              setattr(succ(xp),yp+hhe,shadow,brt);
                            end;
                          textattr:=dtx;
                          showmouse;
                        end;
                  end
                else
                  if destat=true then
                    begin
                      if (wx>=xp+minx) and (wx<=xp+maxx) and (wy>=yp+miny) and (wy<=yp+maxy) and (aktiv<bdw) then
                        begin
                          hidemouse;
                          obj_call(aktiv,11,wx,wy,xp,yp);
                          new_active(bdw,banz,bypos,xp,yp,devon,aktiv,minx,miny,maxx,maxy,keyev,def,flag);
                          showmouse;
                        end
                      else
                        begin
                          dn:=0;
                          for q:=debis downto devon do
                            if q<>aktiv then
                              begin
                                obj_call(q,1,0,0,xp,yp);
                                if (wx>=xp+byteout[0]) and (wx<=xp+byteout[2])
                                  and (wy>=yp+byteout[1]) and (wy<=yp+byteout[3]) then dn:=q;
                              end;
                          if dn>0 then
                            begin
                              hidemouse;
                              if aktiv>=bdw then but_invert(def,bdw,bypos,schwarzaufweiss,1)
                                else obj_call(aktiv,7,0,0,xp,yp);
                              aktiv:=dn;
                              get_activate(aktiv,xp,yp,minx,miny,maxx,maxy,keyev);
                              byteout[0]:=devon;
                              byteout[1]:=debis;
                              obj_call(aktiv,2,wx,wy,xp,yp);
                              new_active(bdw,banz,bypos,xp,yp,devon,aktiv,minx,miny,maxx,maxy,keyev,def,flag);
                              showmouse;
                            end;
                        end;
                    end;
            end;
          if kbshift(ALT) then
            if (aktiv<bdw) and (keyev=1) then
              begin
                obj_call(aktiv,7,0,0,xp,yp);
                aktiv:=bdw+def;
                but_invert(def,bdw,bypos,hellaufweiss,1);
                mem[$0040:$0017]:=mem[$0040:$0017] and (255-ALT);
                void.bo:=kbshift(ALT);
              end;
          if keypressed then
            begin
              taste:=readkey;
              if keypressed then
                begin
                  taste:=readkey;
                  if kbshift(LSHIFT) or kbshift(RSHIFT) then
                    case taste of
                      #15: begin
                             hidemouse;
                             if aktiv>=bdw then
                               begin
                                 but_invert(def,bdw,bypos,schwarzaufweiss,0);
                                 dec(def);
                                 dec(aktiv);
                                 if def<0 then
                                   begin
                                     if destat then
                                       begin
                                         def:=0;
                                         but_invert(def,bdw,bypos,schwarzaufweiss,1);
                                         obj_activate(aktiv,xp,yp,1,minx,miny,maxx,maxy,keyev);
                                         new_active(bdw,banz,bypos,xp,yp,devon,aktiv,minx,miny,maxx,maxy,keyev,def,flag);
                                       end
                                     else
                                       begin
                                         def:=banz;
                                         aktiv:=bdw+banz;
                                         but_invert(def,bdw,bypos,hellaufweiss,1);
                                       end;
                                   end
                                 else but_invert(def,bdw,bypos,hellaufweiss,1);
                               end
                             else
                               begin
                                 obj_call(aktiv,7,0,0,xp,yp);
                                 dec(aktiv);
                                 if aktiv<devon then
                                   begin
                                     but_invert(def,bdw,bypos,schwarzaufweiss,0);
                                     def:=banz;
                                     aktiv:=bdw+banz;
                                     but_invert(def,bdw,bypos,hellaufweiss,1);
                                   end
                                 else
                                   begin
                                     obj_activate(aktiv,xp,yp,1,minx,miny,maxx,maxy,keyev);
                                     new_active(bdw,banz,bypos,xp,yp,devon,aktiv,minx,miny,maxx,maxy,keyev,def,flag);
                                   end;
                               end;
                             showmouse;
                           end;
                      #72: if yp>1 then
                             void.bo:=move_box(xp,yp,xp,pred(yp),hhe,brt,cflag,banz,def,1,xp,yp,bypos);
                      #80: if yp+hhe-cflag<anzline then
                             void.bo:=move_box(xp,yp,xp,succ(yp),hhe,brt,cflag,banz,def,1,xp,yp,bypos);
                      #75: if xp>1 then
                             void.bo:=move_box(xp,yp,pred(xp),yp,hhe,brt,cflag,banz,def,1,xp,yp,bypos);
                      #77: if xp+brt-cflag<anzcol then
                             void.bo:=move_box(xp,yp,succ(xp),yp,hhe,brt,cflag,banz,def,1,xp,yp,bypos);
                    else
                      if aktiv<bdw then
                        begin
                          hidemouse;
                          obj_call(aktiv,10,KEY_SPEC,ord(taste),xp,yp);
                          new_active(bdw,banz,bypos,xp,yp,devon,aktiv,minx,miny,maxx,maxy,keyev,def,flag);
                          showmouse;
                        end;
                    end
                  else
                    case taste of
                      #59: begin
                             hidemouse;
                             oms:=m_stat;
                             hx:=wherex;  hy:=wherey;
                             byteout[2]:=H_DIAL;
                             if length(dwork[0])>6 then byteout[3]:=ord(dwork[0,7])
                               else byteout[3]:=0;
                             mp_dis(M_HELP,oms);
                             gotoxy(hx,hy);
                             showmouse;
                           end;
                    else
                      if aktiv<bdw then
                        begin
                          hidemouse;
                          obj_call(aktiv,10,KEY_SPEC,ord(taste),xp,yp);
                          new_active(bdw,banz,bypos,xp,yp,devon,aktiv,minx,miny,maxx,maxy,keyev,def,flag);
                          showmouse;
                        end;
                    end;
                end
              else
                case taste of
                  #9: begin
                        hidemouse;
                        if aktiv>=bdw then
                          begin
                            but_invert(def,bdw,bypos,schwarzaufweiss,0);
                            inc(def);
                            inc(aktiv);
                            if def>banz then
                              begin
                                if destat then
                                  begin
                                    def:=banz;
                                    aktiv:=devon;
                                    but_invert(def,bdw,bypos,schwarzaufweiss,1);
                                    obj_activate(aktiv,xp,yp,0,minx,miny,maxx,maxy,keyev);
                                    new_active(bdw,banz,bypos,xp,yp,devon,aktiv,minx,miny,maxx,maxy,keyev,def,flag);
                                  end
                                else
                                  begin
                                    def:=0;
                                    aktiv:=bdw;
                                    but_invert(def,bdw,bypos,hellaufweiss,1);
                                  end;
                              end
                            else but_invert(def,bdw,bypos,hellaufweiss,1);
                          end
                        else
                          begin
                            obj_call(aktiv,7,0,0,xp,yp);
                            inc(aktiv);
                            if aktiv>=bdw then
                              begin
                                but_invert(def,bdw,bypos,schwarzaufweiss,0);
                                def:=0;
                                but_invert(def,bdw,bypos,hellaufweiss,1);
                              end
                            else
                              begin
                                obj_activate(aktiv,xp,yp,0,minx,miny,maxx,maxy,keyev);
                                new_active(bdw,banz,bypos,xp,yp,devon,aktiv,minx,miny,maxx,maxy,keyev,def,flag);
                              end;
                          end;
                        showmouse;
                      end;
                  #13: flag:=def;
                  #27: flag:=M_CANCEL;
                else
                  if (aktiv<bdw) and (keyev>0) then
                    begin
                      hidemouse;
                      obj_call(aktiv,10,KEY_NORM,ord(taste),xp,yp);
                      new_active(bdw,banz,bypos,xp,yp,devon,aktiv,minx,miny,maxx,maxy,keyev,def,flag);
                      showmouse;
                    end
                  else
                    begin
                      dpos:=pos(upchar(taste),dhkstr);
                      if (dpos>0) and (dpos<=succ(banz)) then flag:=pred(dpos)
                      else
                        begin
                          dn:=0;
                          for q:=debis downto devon do
                            if q<>aktiv then
                              begin
                                obj_call(q,5,KEY_NORM,ord(taste),xp,yp);
                                if byteout[0]=1 then dn:=q;
                              end;
                          if dn>0 then
                            begin
                              hidemouse;
                              if aktiv>=bdw then but_invert(def,bdw,bypos,schwarzaufweiss,1)
                                else obj_call(aktiv,7,0,0,xp,yp);
                              aktiv:=dn;
                              get_activate(aktiv,xp,yp,minx,miny,maxx,maxy,keyev);
                              byteout[0]:=devon;
                              byteout[1]:=debis;
                              obj_call(aktiv,2,0,0,xp,yp);
                              new_active(bdw,banz,bypos,xp,yp,devon,aktiv,minx,miny,maxx,maxy,keyev,def,flag);
                              showmouse;
                            end
                          else
                            if aktiv<bdw then
                              begin
                                hidemouse;
                                obj_call(aktiv,10,KEY_NORM,ord(taste),xp,yp);
                                new_active(bdw,banz,bypos,xp,yp,devon,aktiv,minx,miny,maxx,maxy,keyev,def,flag);
                                showmouse;
                              end;
                        end;
                    end;
                end;
            end;
          with JRECT do
            begin
              xl:=xp;
              yo:=yp;
              xr:=xp+brt;
              yu:=yp+hhe;
              if not(farbe) then
                begin
                  dec(xr);
                  dec(yu);
                end;
            end;
          menu_event(S_DIALOG);
        until flag<>M_NOP;
        if p1<>nil then freemem(p1,l1);
        derrjmp:
        hidemouse;
        put_vioram(xp,yp,brt,hhe,cflag);
        textattr:=dummy;
        gotoxy(owx,owy);
        repeat until mouseb=0;
        showmouse;
        if (dbits and D_MOVE)>0 then
          begin
            byteout[0]:=xp;
            byteout[1]:=yp;
          end;
        dialog_do:=flag;
      end
    else dialog_do:=E_NREENT;
  end;