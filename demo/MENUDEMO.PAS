program MenuDemo;

  uses crt,asv;

  var ret,pret : integer;
      dcount   : byte;
      info     : string;

{$F+}
procedure hilfe(wnum : byte); {$F-}

  begin
    gotoxy(1,_scry);
    write('                                                                        ');
    gotoxy(1,_scry);
    if wnum=M_HELP then
      begin
        write('erweiterte Hilfsfunktion (Demo): ');
        case byteout[2] of
          H_PULL: write('Pulldown-MenÅ');
           H_POP: write('Popup-MenÅ   ');
          H_DIAL: write('Dialogbox    ');
        end;
        write('  (Bitte drÅcken Sie eine Taste)');
        void.c:=readkey;
        gotoxy(1,_scry);
        write('                                                                              ');
      end
    else
      case wnum of
         1..8,27: write('Funktion nicht anwÑhlbar ...');
              10: write('Beendet das Demo und kehrt zum Betriebssystem zurÅck');
              13: write('Alertbox-Demo');
              14: write('Gibt eine Fehlermeldung aus');
              15: write('Schaltet zwischen deutschen und englischen Fehlermeldungen um');
              17: write('FÅgt im MenÅ "Fenster" einen MenÅeintrag an');
              18: write('Lîscht im MenÅ "Fenster" den letzten MenÅeintrag');
              20: write('Dieser Eintrag ist disabled ...');
              21: write('Ruft ein PopUp-MenÅ auf');
              23: if menu_stat(23)=A_CHECKED then write('Dieser Eintrag kann deselektiert werden')
                    else write('Dieser Eintrag kann selektiert werden');
              26: write('Gibt die Copyright-Meldung aus');
              29: write('Dialogbox-Demo (lÑuft nur mit mind. 80x25 Zeichen !)');
          31..39: write('Platzhalter fÅr bis zu 9 geladene Dateien');
        P_OFFS+0: write('PopUp-MenÅ-Eintrag Nr.1');
        P_OFFS+2: write('PopUp-MenÅ-Eintrag Nr.2');
        P_OFFS+3: if work[25,1]=#0 then write('Dieser Eintrag kann selektiert werden')
                    else write('Dieser Eintrag kann deselektiert werden');
        P_OFFS+4: write('Dieser Eintrag kann nicht angewÑhlt werden ...');
        P_OFFS+6: write('Der 5. PopUp-MenÅ-Eintrag');
      end;
  end;


procedure work_init;

  begin
    { Die MenÅs 'Datei' und 'Fenster' sollten immer in Ñhnlicher Weise
      wie hier angegeben aufgebaut sein (MS macht's auch so !)         }
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
    work[20]:='Das ist #disabled...';
    work[21]:='#PopUp-MenÅ...';
    work[22]:='-';
    work[23]:='#Select it !';
    work[24]:='';
    work[25]:='#Fenster';
    work[26]:='#Info...';
    work[27]:='#Hilfsfunktion...';
    work[28]:='-';
    work[29]:='#Einstellungen...';
    work[30]:='-';
    work[31]:='#1 DATEI1.EXT';
    work[32]:='';
    work[33]:='';
  end;


begin
  work_init;
  menu_init;
  info:=' ASV - Application Services v1.0|Copyright (c)08.09.1990 by SoftDesign|     written by Thomas Much||';
  info:=info+'  Koppelweg 16a * 3201 Diekholzen|       Tel.: 05121/262007||  fÅr TurboPascal 5.0/5.5 (49,90DM)';
  work[20]:=#255#255#2#0;
  work[21]:='POPUP-MENö';
  work[22]:=#0'Nummer #1';
  work[23]:='-';
  work[24]:=#0'Nummer #2';
  work[25]:=chr(A_CHECKED)+'#Select it !';
  work[26]:=chr(A_DISABLED)+'Das ist #disabled...';
  work[27]:='-';
  work[28]:=#0'Das ist ein #Test !';
  work[29]:='';
  work[30]:='';
  menu_setp(hilfe);
  menu_draw;
  menu_attr(20,A_DISABLED);
  dcount:=50;
  lowvideo;
  repeat
    repeat
      ret:=menu_watch;
      case ret of
        M_NOP: { andere Routinen des Programms };
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
                menu_add('#'+chr(dcount)+' DATEI'+chr(dcount)+'.EXT');
                inc(dcount);
              end
            else void.i:=form_alert('','Mehr als 9 Dateien sind nicht mîglich !','  #OK  ',0);
        18: begin
              menu_inquire;
              if intout[7]>30 then
                begin
                  menu_delete(intout[7]);
                  dec(dcount);
                end
              else void.i:=form_alert('','Es sind bereits alle Dateien gelîscht !','  #OK  ',0);
            end;
        21: begin
              pret:=menu_pop(20);
              if pret=3 then
                if work[25,1]=#0 then work[25,1]:=chr(A_CHECKED)
                  else work[25,1]:=#0;
              if pret>=M_OK then work[20,3]:=chr(pret);
            end;
        23: if menu_stat(23)=A_CHECKED then menu_attr(23,A_NCHECKED)
              else menu_attr(23,A_CHECKED);
        26: void.i:=form_alert('INFO',info,'  OK  ',3);
        29: begin
              work[0]:=#255#255#0#0#79#24;
              work[1]:='EINSTELLUNGEN';
              work[2]:=#10#5'Das ist eine';
              work[3]:=#12#7'recht gro·e';
              work[4]:=#8#10'Dialogbox.';
              work[5]:='';
              work[6]:='  #OK  ';
              work[7]:='#Abbruch';
              work[8]:=' #Hilfe ';
              work[9]:='';
              work[10]:='';
              void.i:=dialog;
            end;
      end;
    until ret=10;
    ret:=form_alert('ENDE','Wollen Sie das Programm wirklich verlassen ?','  #Ja  | #Nein ',1);
  until ret=0;
  menu_kill;
end.