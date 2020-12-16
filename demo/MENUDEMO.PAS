program MenuDemo;

  {$M $2000,0,$2000}
  { fÅr den Aufruf der DOS-Shell }

  uses crt,dos,asv;

  var ret,pret,but          : integer;
      clock_jnr,pmt_jnr     : integer;
      dcount,txattr,pmt_y   : byte;
      has,was,rbut1,rbut2   : byte;
      pbut1,pbut2,q,saw,haw : byte;
      oldattr,dbut          : byte;
      info,name,strasse,ort : string;
      slx,sly               : word;
      xo,yo                 : array [0..2] of byte;
      dbuts                 : array [0..4] of byte;
      pmt_sr                : searchrec;
      crect,drect,drc2,drc3 : JREC;
      { die Variablen werden dort erklÑrt, wo sie verwendet werden }


{$F+}
procedure clock(job,stat : byte); {$F-} { Ein Multitasking-Job! }
  var h,m,s,s1 : word;
      zeit     : string;
      dtatr    : byte;

  begin
    gettime(h,m,s,s1);
    if stat in [S_MENU,S_DIALOG] then
      if rc_intersect(crect,JRECT) then exit;
    zeit:=itoa(h)+':';
    if h<10 then zeit:='0'+zeit;
    if m<10 then zeit:=zeit+'0';
    zeit:=zeit+itoa(m)+':';
    if s<10 then zeit:=zeit+'0';
    zeit:=zeit+itoa(s);
    dtatr:=textattr;
    textattr:=saw;
    printxy(_scrx-9,1,zeit);
    textattr:=dtatr;
  end;


{$F+}
procedure pmt_demo(job,stat : byte); {$F-} { noch ein Job }
  var dtatr : byte;

  begin
    if stat in [S_MENU,S_DIALOG] then
      if rc_intersect(drect,JRECT) then exit;
    if pmt_y>_scry-6 then
      begin
        rc_copy(drc2,drc3);
        pmt_y:=_scry-6;
      end;
    dtatr:=textattr;
    textattr:=was;
    if doserror=0 then
      begin
        printxy(61,pmt_y,'   '+pmt_sr.name+space(15-length(pmt_sr.name)));
        findnext(pmt_sr);
      end
    else
      begin
        printxy(61,pmt_y,'+++            +++');
        findfirst('*.*',0,pmt_sr);
      end;
    textattr:=dtatr;
    inc(pmt_y);
  end;


{$F+}
procedure hilfe(wnum : byte); {$F-}

  begin
    textattr:=was;
    printxy(1,_scry,space(_scrx));
    { die unterste Zeile wird gelîscht }
    gotoxy(1,_scry);
    { eventuelle Hilfsmeldungen werden unten links geschrieben }
    if wnum=M_HELP then
      begin
        { erweiterte Hilfsfunktion }
        case byteout[2] of
          H_PULL: ;
           H_POP: ;
          H_DIAL: ;
        end;
      end
    else
      case wnum of
         0,12,25: write('<'#17#16#30#31'>, <'#17'Ÿ> oder markierte Buchstaben benutzen');
               2: write('LÑdt ein ausfÅhrbares Programm nach');
               4: write('Fileselect-Demo');
               8: write('Ruft die DOS-Shell auf');
              10: write('Beendet das Demo und kehrt zum Betriebssystem zurÅck');
              13: write('Alertbox-Demo');
              14: write('Gibt eine Fehlermeldung aus');
              15: if menu_text(15)='Fehlermeldungen: Deutsch' then write('Schaltet auf englische Fehlermeldungen um')
                    else write('Schaltet auf deutsche Fehlermeldungen um');
              17: write('FÅgt im MenÅ "Fenster" einen MenÅeintrag an');
              18: write('Lîscht im MenÅ "Fenster" den letzten MenÅeintrag');
              21: write('Ruft ein PopUp-MenÅ auf');
              23: if menu_stat(23)=A_CHECKED then write('Schaltet auf Pulldown-MenÅs um')
                    else write('Schaltet auf Dropdown-MenÅs um');
              26: write('Gibt die Copyright-Meldung aus');
              27: write('Druckt den Bildschirminhalt aus');
              29: write('Dialogbox-Demo (incl. Push-/Radiobuttons & Eingabefeldern)');
          31..39: write('Platzhalter fÅr bis zu 9 geladene Dateien');
        { nun folgen die Hilfsmeldungen des PopUp-MenÅs }
        P_OFFS+0: write('Demonstriert die ASV-interne Farbeinstellbox');
        P_OFFS+2: write('PopUp-MenÅ-Eintrag Nr.2');
        P_OFFS+3: if work[35,1]=#0 then write('Schaltet das Pseudo-Multitasking-Demo ein')
                    else write('Schaltet das PMT-Demo aus');
        P_OFFS+4: write('éndert die PrioritÑt des PMT-Demo-Jobs');
        P_OFFS+6: write('Gibt Informationen Åber den Speicher etc. aus');
      end;
    if wherex=1 then
      begin
        dbut_draw(dbuts[3]);
        dbut_draw(dbuts[4]);
      end;
  end;


procedure work_init;

  begin
    { die MenÅs 'Datei' und 'Fenster' sollten immer in Ñhnlicher
      Weise wie hier aufgebaut sein (MS macht's auch so !)       }
    work[0]:='#Datei';
    work[1]:='#Neu...';
    work[2]:='#Laden';
    work[3]:='#Speichern';
    work[4]:='Speichern #unter...';
    work[5]:='#Alles speichern';
    work[6]:='S#chlie·en';
    work[7]:='-';
    work[8]:='#Betriebssystem...';
    work[9]:='-';
    work[10]:='#Ende';
    work[11]:='';
    work[12]:='#Extras';
    work[13]:='#Alertbox...';
    work[14]:='Fehlermeldung...';
    work[15]:='#Fehlermeldungen: Deutsch';
    work[16]:='-';
    work[17]:='Eintrag #hinzufÅgen';
    work[18]:='#Eintrag lîschen';
    work[19]:='-';
    work[20]:='Das ist d#isabled...';
    work[21]:='#PopUp-MenÅ...';
    work[22]:='-';
    work[23]:='#Dropdown-MenÅs';
    work[24]:='';
    work[25]:='#Fenster';
    work[26]:='#öber ASV...';
    work[27]:='#Hardcopy';
    work[28]:='-';
    work[29]:='#Einstellungen...';
    work[30]:='-';
    work[31]:='#1 DATEI1.EXT';
    work[32]:='';
    work[33]:='';
    menu_space(S_MID);
    { Zwischenraum der MenÅtitel = mittel }
    menu_init;
    { die MenÅstruktur wird angemeldet ... }
    menu_attr(1,A_DISABLED);
    menu_attr(3,A_DISABLED);
    menu_attr(5,A_DISABLED);
    menu_attr(6,A_DISABLED);
    menu_attr(20,A_DISABLED);
    { ... und die nicht benîtigten EintrÑge disabled }
    with crect do
      begin
        xl:=_scrx-9;
        yo:=1;
        xr:=xl+7;
        yu:=1;
      end;
    with drect do
      begin
        xl:=61;
        yo:=4;
        xr:=78;
        yu:=_scry-6;
      end;
    { hier werden die Vergleichs-Rechtecke fÅr die Jobs belegt... }
    with drc2 do
      begin
        xl:=61;
        yo:=5;
        xr:=78;
        yu:=_scry-6;
      end;
    with drc3 do
      begin
        xl:=61;
        yo:=4;
      end;
    { ...und hier die Rechtecke zum Bildschirm-Kopieren }
    dbuts[0]:=dbut_insert(48,22,B_FSINGLE,was,B_TOUCH,B_MNONE,B_KNONE,B_KNONE,' #'+chr(has)+'D#'#0'rei ');
    dbuts[1]:=dbut_insert(58,22,B_FHALF,was,B_TOUCH,B_MNONE,B_KNONE,B_KNONE,'#'+chr(saw)+'D#'#0'esk#'+chr(haw)+'t#'#0'op');
    dbuts[2]:=dbut_insert(69,22,B_FDOUBLE,has,B_TOUCH,B_MNONE,B_KNONE,B_KNONE,
                          ' B#'+chr(has)+'u#'#0't#'+chr(has)+'t#'#0'o#'+chr(has)+'n#'#0's ');
    dbuts[3]:=dbut_insert(1,_scry,B_FNONE,0,B_CLICK,B_MNONE,KEY_SPEC,K_F10,' #'+chr(has)+'ALT#'#0' MenÅ ');
    dbuts[4]:=dbut_insert(11,_scry,B_FNONE,0,B_CLICK,26,B_KNONE,B_KNONE,' #'+chr(has)+'Shift-F10#'#0' Info ');
    { die Desktop-Buttons werden initialisiert }
  end;


{$F+}
procedure sltext(wnum,com,dat1,dat2 : byte); {$F-}
  { das ist ein Dialogobjekt ! }
  var dtxta,num2,x,y : byte;
      txt            : string;

  begin
    dtxta:=textattr;
    { Textattribute merken }
    x:=byteout[2]+ord(work[wnum,3]);
    y:=byteout[3]+ord(work[wnum,4]);
    { Position des Textes ermitteln }
    txt:=right(work[wnum],length(work[wnum])-4);
    { Text ermitteln }
    num2:=wnum+7;
    { die Slider fÅr dieses Objekt haben einen Offset von 7 EintrÑgen }
    case com of
      0: begin
           if wnum=21 then form_objcall(_vslider,num2,0,dat1,dat2)
             else form_objcall(_hslider,num2,0,dat1,dat2);
           { das Zeichenkommando wird an die Slider-Objekte weitergegeben }
           textattr:=txattr;
           printxy(x,y,txt+itoa(form_getslider(num2)));
           { Text (hier: Sliderposition) ausgeben }
         end;
      1: begin
           byteout[0]:=ord(work[num2,3]);
           byteout[1]:=ord(work[num2,4]);
           { linke obere Ecke des Aktivierungs-Rechtecks }
           if wnum=21 then
             begin
               byteout[2]:=byteout[0];
               byteout[3]:=pred(byteout[1]+ord(work[num2,5]));
               { vertikaler Slider }
             end
           else
             begin
               byteout[2]:=pred(byteout[0]+ord(work[num2,5]));
               byteout[3]:=byteout[1];
               { horizontaler Slider }
             end;
           byteout[4]:=0;
           { keine erweiterten Tastaturevents }
         end;
   2,11: begin
           if wnum=21 then form_objcall(_vslider,num2,com,dat1,dat2)
             else form_objcall(_hslider,num2,com,dat1,dat2);
           { Aktivierung bzw. Mausevent weitergeben,
             bei diesem Objekt ist beides zusammengefa·t }
           textattr:=txattr;
           printxy(x,y,txt+itoa(form_getslider(num2))+' ');
           { neue Sliderposition ausgeben }
         end;
      3: if byteout[4]=0 then byteout[3]:=succ(wnum)
           else byteout[3]:=pred(wnum);
         { keine <Alt>-Anwahl mîglich, nÑchstes Objekt aktivieren }
      5: byteout[0]:=0;
         { keine Hotkey-Anwahl }
     10: byteout[3]:=0;
         { keine Tastaturevents }
    end;
    textattr:=dtxta;
    { Textattribute restaurieren }
  end;


procedure dialogbox;
  var dnummer : integer;

  begin
    dnummer:=form_objins(sltext);
    { wir versuchen, das eigene Objekt (s.o.) anzumelden }
    if dnummer=F_ERROR then exit;
    { leider kein Objektspeicher mehr frei ... }
    work[1]:='EINSTELLUNGEN';
    { der Titel der Dialogbox }
    work[2]:=dframe(3,2,39,5,'');
    work[3]:=dframe(3,8,39,5,'PeripheriegerÑte');
    work[4]:=#255#0#44#2#22#5'Festplatte';
    work[5]:=dframe(44,8,22,5,'Monitor');
    work[6]:=dtext(6,14,'Mit wenigen Programmzeilen kann man');
    work[7]:=#29#15'beeindruckende Effekte erzielen.';
    { der Rahmen- und Textteil der Dialogbox }
    work[8]:='';
    work[9]:=dalpha(5,3,24,23,B_ENABLED,'#Name:   ',name);
    work[10]:=dalpha(5,4,24,30,B_ENABLED,'#Stra·e: ',strasse);
    work[11]:=dalpha(5,5,24,30,B_DISABLED,'#Wohnort:',ort);
    work[12]:=dpush(5,9,pbut1,'Speicher#erweiterung');
    work[13]:=dpush(5,10,B_DISABLED,'Maus/#Lightpen');
    work[14]:=dpush(5,11,pbut2,'#Drucker/Scanner/Digitalisierer');
    work[15]:=dradio(46,3,0,B_OFF,'bis 2#0 MB');
    work[16]:=dradio(46,4,0,B_OFF,'#20-40 MB');
    work[17]:=dradio(46,5,0,B_OFF,'mehr als #40 MB');
    work[18]:=dradio(46,9,1,B_OFF,'#Monochrom');
    work[19]:=dradio(46,10,1,B_OFF,'#Farbe/Graust.');
    work[20]:=dradio(46,11,1,B_DISABLED,'Gro·#bildschirm');
    { diese Objekte werden von ASV zur VerfÅgung gestellt }
    work[21]:=#255+chr(dnummer)+#14#16'Y: ';
    work[22]:=#255+chr(dnummer)+#6#16'X: ';
    { so bastelt man sich eigene Objekt-EintrÑge ! }
    work[23]:='  #OK  ';
    work[24]:='#Abbruch';
    work[25]:=' #Hilfe ';
    { die Exit-Buttons der Dialogbox }
    work[26]:='';
    work[27]:='';
    { hier ist fÅr die Funktion dialog die Box zu Ende ! }
    work[28]:=dvslider(64,13,4,sly,1);
    work[29]:=dhslider(4,17,60,slx,57);
    { diese beiden EintrÑge werden vom Objekt sltext benîtigt,
      um die entsprechenden Slider aufrufen zu kînnen (s.o.)   }
    form_setrb(rbut1,1);
    form_setrb(rbut2,1);
    { hier werden die Radiobuttons aktiviert }
    but:=0;
    { der Default-Exitbutton ist zunÑchst "OK" }
    repeat
      work[0]:=chr(xo[0])+chr(yo[0])+chr(but)+#0#69#21;
      { work[0] folgt erst hier, damit die Box nach der erweiterten
        Hilfsfunktion an der alten Stelle erscheint (xo/yo !)       }
      but:=dialog;
      { Zeichnen, Auswerten & Lîschen der Dialogbox }
      xo[0]:=byteout[0];  yo[0]:=byteout[1];
      { verÑnderte Position merken }
      if but=0 then
        begin
          { wenn der "OK"-Button angewÑhlt wurde, merken
            wir uns hier die neuen Werte                 }
          name:=form_getan(9);
          strasse:=form_getan(10);
          ort:=form_getan(11);
          pbut1:=form_getpb(12);
          pbut2:=form_getpb(14);
          rbut1:=form_findrb(9,23,0);
          rbut2:=form_findrb(18,20,1);
          sly:=form_getslider(28);
          slx:=form_getslider(29);
        end;
      if but=2 then void.i:=form_alert('HILFE','Das sollte eine erweiterte Hilfsfunktion sein.','  #OK  ',0);
      { wurde der "Hilfe"-Button angewÑhlt, erscheint eine neue Alertbox }
    until but<>2;
    { solange ausfÅhren, bis ein anderer als der "Hilfe"-Button
      angewÑhlt wurde (schlie·t <Esc> etc. mit ein !)           }
    void.i:=form_objdel(dnummer);
    { Objektspeicher wieder freigeben }
  end;


procedure sys_info;

  begin
    menu_inquire;
    work[0]:=chr(xo[1])+chr(yo[1])+#0#0#52#15;
    work[1]:='SYSTEMINFORMATION';
    work[2]:=dframe(2,2,20,6,'Bildschirm');
    work[3]:=dframe(23,2,27,6,'Maus');
    work[4]:=dframe(2,8,48,4,'System');
    if color then work[5]:=dtext(4,3,'Farbmonitor')
      else work[5]:=dtext(4,3,'Monochrommonitor');
    work[6]:=dtext(4,5,'Zeilen:  '+itoa(_scry));
    work[7]:=dtext(4,6,'Spalten: '+itoa(_scrx));
    if mouse then
      begin
        work[8]:=dtext(25,3,'Tasten:            '+itoa(byteout[10]));
        work[9]:=dtext(25,4,'Empfindlichkeit X: '+itoa(intout[8]));
        work[10]:=dtext(25,5,'Empfindlichkeit Y: '+itoa(intout[9]));
        work[11]:=dtext(25,6,'Schwellenwert:     '+itoa(intout[10]));
      end
    else
      begin
        work[8]:=dtext(25,3,'Keine Maus installiert.');
        work[9]:=dtext(25,4,'');
        work[10]:=dtext(25,5,'');
        work[11]:=dtext(25,6,'');
      end;
    work[12]:=dtext(4,9,'Speicherausbau:  '+itoa(intout[3])+' KByte RAM (ohne EMS).');
    work[13]:=dtext(4,10,'Land (ASV):      ');
    case intout[0] of
      L_USA: work[13]:=work[13]+'USA';
      L_GB:  work[13]:=work[13]+'England';
      L_FRG: work[13]:=work[13]+'Deutschland';
    else
      work[13]:=work[13]+'unbekannt';
    end;
    work[14]:='';
    work[15]:='  #OK  ';
    work[16]:='';
    work[17]:='';
    void.i:=dialog;
    xo[1]:=byteout[0];  yo[1]:=byteout[1];
  end;


procedure bildschirm;

  begin
    clsp(was,C_WDOT);
    { Hintergrund zeichnen }
    menu_draw;
    { MenÅleiste zeichnen }
    hidemouse;
    hilfe(M_EXIT);
    { allg. Informationen anzeigen lassen (s.o.) }
    showmouse;
    hide;
  end;


procedure exec_file;
  var fname,sdummy : string;
      derr         : integer;

  begin
    fname:=fileselect('LADEN','',' *.Exe, *.com ','');
    if length(fname)>0 then
      if exist(fname) then
        begin
          job_disable;
          hidemouse;
          clrscr;
          swapvectors;
          exec(fname,'');
          swapvectors;
          derr:=doserror;
          asv_init;
          { sollte nach jedem exec() etc. aufgerufen werden }
          if derr<>0 then void.i:=form_alert('PROGRAMM STARTEN','Fehler #'+itoa(derr)+
                                             ' beim Starten von '+fname+' !','  #OK  ',0)
          else
            begin
              derr:=dosexitcode;
              sdummy:='Programm '+fname+' beendet. (ExitCode: '+itoa(lo(derr))+')';
              case hi(derr) of
                1: begin
                     q:=length(sdummy)-37;
                     if q<0 then q:=0;
                     sdummy:=sdummy+'|'+space(q shr 1)+'[Abbruch durch <Ctrl+C>/<Ctrl+Break>]';
                   end;
                2: begin
                     q:=length(sdummy)-28;
                     if q<0 then q:=0;
                     sdummy:=sdummy+'|'+space(q shr 1)+'[Abbruch wegen GerÑtefehler]';
                   end;
                3: begin
                     q:=length(sdummy)-32;
                     if q<0 then q:=0;
                     sdummy:=sdummy+'|'+space(q shr 1)+'[Programm verbleibt im Speicher]';
                   end;
              end;
              void.i:=form_alert('PROGRAMM STARTEN',sdummy,'  #OK  ',0);
            end;
          bildschirm;
          { Bildschirm neu aufbauen }
          job_enable;
        end
      else void.i:=form_alert('LADEN','Datei "'+fname+'" nicht gefunden.','  #OK  ',0);
  end;


begin
  was:=byteout[0];
  saw:=byteout[1];
  has:=byteout[2];
  haw:=byteout[3];
  txattr:=byteout[15];
  { beim Programmstart sind die Ausgabefelder initialisiert, s. menu_inquire }
  work_init;
  { MenÅ anmelden etc. (s.o.) }
  menu_attr(23,A_CHECKED);
  { Dropdown-MenÅs einschalten }
  info:='  ASV - Application Services '+ftoa(_asv)+'|Copyright (c)';
  if byteout[12]<10 then info:=info+'0';
  info:=info+itoa(byteout[12])+'.';
  if byteout[13]<10 then info:=info+'0';
  info:=info+itoa(byteout[13])+'.';
  info:=info+itoa(byteout[14]+1900)+' by Softdesign|     written by Thomas Much||  ';
  info:=info+'Koppelweg 16a * 3201 Diekholzen|      Tel.: (05121) 26 20 07||  fÅr TurboPascal 5.0-6.0 (DM 49,-)';
  { der Textstring fÅr eine Alertbox }
  work[30]:=#255#255#2#0;
  work[31]:='POPUP-MENö';
  work[32]:=#0'#Farben...';
  work[33]:='-';
  work[34]:=#0'Nummer #2';
  work[35]:=#0'PMT-#Demo...';
  work[36]:=chr(A_DISABLED)+'#PrioritÑt...';
  work[37]:='-';
  work[38]:=#0'System#information...';
  work[39]:='';
  work[40]:='';
  { die EintrÑge fÅr ein Popup-MenÅ; da das MenÅ schon angemeldet ist,
    kann das work[]-Feld nun anderweitig belegt werden                 }
  menu_setp(hilfe);
  { Hilfsfunktion einschalten }
  form_errexit(X_OK);
  { bei einem Fehler wird keine Abbruchmîglichkeit gegeben }
  dcount:=50;
  bildschirm;
  for q:=0 to 2 do
    begin
      xo[q]:=255;  yo[q]:=255;
    end;
  { die Dialogboxen erscheinen zunÑchst zentriert }
  pbut1:=B_ON;  pbut2:=B_NXON;  rbut1:=15;  rbut2:=18;
  if color then inc(rbut2);
  { die Werte fÅr Buttons einer Dialogbox }
  name:='';
  strasse:='Musterstra·e 08/15 abcDEF 1234567890';
  ort:='4711 Musterstadt';
  { die Strings fÅr alphanumerische Eingabefelder }
  slx:=10;  sly:=0;
  { die Sliderpositionen in einer Dialogbox }
  clock_jnr:=job_add(clock,100,S_ALL);
  { hier wird ein Multitasking-Job angemeldet und gestartet }
  oldattr:=saw;
  repeat
    repeat
      ret:=menu_watch;
      { MenÅanwahl Åberwachen }
      if byteout[0]=KEY_SPEC then if byteout[1]=93 then ret:=26;
      if ret>=B_OFFS then
        begin
          dbut:=ret-B_OFFS;
          { hier werden die Desktop-Buttons ausgewertet }
          if dbut=dbuts[0] then void.i:=form_alert('','Desktop-Button Nr. 1...','  #OK  ',0)
          else if dbut=dbuts[1] then void.i:=form_alert('','Desktop-Button Nr. 2...','  #OK  ',0)
          else if dbut=dbuts[2] then void.i:=form_alert('','Desktop-Button Nr. 3...','  #OK  ',0);
        end
      else
        case ret of
           M_NOP: { andere Routinen des Programms };
           2: exec_file;
           4: void.s:=fileselect('SPEICHERN UNTER','','','noname.pas');
           8: if exist('COMMAND.COM') then
                begin
                  hidemouse;
                  clrscr;
                  swapvectors;
                  exec('COMMAND.COM','');
                  swapvectors;
                  asv_init;
                  { sollte nach jedem exec() etc. aufgerufen werden }
                  bildschirm;
                  { Bildschirm neu aufbauen }
                end
              else void.i:=form_alert('BETRIEBSSYSTEM','DOS-Shell "COMMAND.COM" nicht gefunden.','  #OK  ',0);
          13: void.i:=form_alert('ALERTBOX','Diese Alertbox besitzt|  drei Textzeilen|    und vier Buttons.',
                                 ' #1 | #2 | #3 |#Abbruch',3);
          14: form_error(M_ERROR);
          15: begin
                menu_inquire;
                { kann auch mit fn menu_text() programmiert werden }
                if intout[0]=L_FRG then
                  begin
                    form_seterrl(L_GB);
                    menu_rename(15,'Error #messages: English');
                  end
                else
                  begin
                    form_seterrl(L_FRG);
                    menu_rename(15,'#Fehlermeldungen: Deutsch');
                  end;
              end;
          17: if dcount<58 then
                begin
                  if dcount=49 then menu_add('-');
                  { wenn gar kein Eintrag da ist, wird zunÑchst
                    eine Trennlinie eingefÅgt                   }
                  menu_add('#'+chr(dcount)+' DATEI'+chr(dcount)+'.EXT');
                  inc(dcount);
                end
              else void.i:=form_alert('EINTRAG HINZUFöGEN','Mehr als 9 EintrÑge sind nicht mîglich !','  #OK  ',0);
          18: begin
                menu_inquire;
                if intout[7]>30 then
                  begin
                    menu_delete(intout[7]);
                    dec(dcount);
                    if dcount=49 then menu_delete(pred(intout[7]));
                    { wenn der letzte Eintrag gelîscht wurde, wird
                      auch die Trennlinie gelîscht                 }
                  end
                else void.i:=form_alert('EINTRAG LôSCHEN','Es sind bereits alle EintrÑge gelîscht !','  #OK  ',0);
              end;
          21: begin
                menu_setschr(251);
                { Selected-Zeichen Ñndern }
                pret:=menu_pop(30);
                case pret of
                  0: begin
                       but:=colours('FARBEINSTELLUNG',oldattr);
                       if but>=0 then oldattr:=but;
                     end;
                  3: if work[35,1]=#0 then
                       begin
                         work[35,1]:=chr(A_CHECKED);
                         findfirst('*.*',0,pmt_sr);
                         pmt_y:=4;
                         pmt_jnr:=job_add(pmt_demo,0,S_ALL);
                         if pmt_jnr<>J_ERROR then work[36,1]:=#0;
                       end
                     else
                       begin
                         work[35,1]:=#0;
                         work[36,1]:=chr(A_DISABLED);
                         if pmt_jnr<>J_ERROR then void.i:=job_kill(pmt_jnr);
                         clsp(was,C_WDOT);
                         hilfe(M_EXIT);
                         dbut_draw(B_ALL);
                         hide;
                       end;
                     { Selected-Status Ñndern & Job ein-/ausschalten }
                  4: begin
                       work[0]:=chr(xo[2])+chr(yo[2])+#0#0#32#7;
                       work[1]:='PRIORITéT éNDERN';
                       work[2]:='';
                       work[3]:=dalpha(3,2,6,5,B_ENABLED,'Neue #PrioritÑt:',itoa(job_gettime(pmt_jnr)));
                       work[4]:='  #OK  ';
                       work[5]:='#Abbruch';
                       work[6]:='';
                       work[7]:='';
                       but:=dialog;
                       xo[2]:=byteout[0];  yo[2]:=byteout[1];
                       if but=0 then
                         if (atoi(form_getan(3))>=0) and (atoi(form_getan(3))<=65535) then
                           job_settime(pmt_jnr,atoi(form_getan(3)));
                     end;
                  6: sys_info;
                end;
                if pret>=M_OK then work[30,3]:=chr(pret);
                { Default-Eintrag neu bestimmen }
                menu_setschr(7);
              end;
          23: if menu_stat(23)=A_CHECKED then
                begin
                  menu_attr(23,A_NCHECKED);
                  menu_sets(M_PULL+M_NMOVE);
                  { Pulldown-MenÅs einschalten }
                end
              else
                begin
                  menu_attr(23,A_CHECKED);
                  menu_sets(M_DROP);
                  { Dropdown-MenÅs einschalten }
                end;
          26: begin
                kbset(SCRLLOCK);
                kbset(CAPSLOCK);
                { Festbeleuchtung an ... }
                void.i:=form_alert('INFO',info,'  #OK  ',3);
                kbclr(CAPSLOCK);
                kbclr(SCRLLOCK);
                { ... und wieder aus }
              end;
          27: begin
                but:=0;
                while (not(prn_stat(P_ONLINE))) and (but=0) do
                  but:=form_alert('HARDCOPY','Drucker nicht bereit !  ','#Nochmal|#Abbruch',0);
                if but=0 then hardcopy;
              end;
          29: dialogbox;
        end;
    until ret=10;
    { wiederholen, bis "Ende"-Eintrag angewÑhlt wird }
    ret:=form_alert('ENDE','Wollen Sie das Programm wirklich verlassen ?','  #Ja  | #Nein ',1);
  until ret=0;
  { widerholen, bis "OK"-Button angewÑhlt wird }
  menu_kill;
  { MenÅstruktur abmelden und vom Bildschirm lîschen }
  hidemouse;
  { Maus ausschalten (wegen clrscr) }
  clrscr;
end.