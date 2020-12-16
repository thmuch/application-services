{$F+}
procedure hilfe(wnum : byte); {$F-}

  begin
    gotoxy(1,_scry);
    write('                                                                        ');
    { M_EXIT braucht nicht abgefragt zu werden, bei jedem unbekannten
      Wert wird die unterste Bildschirmzeile komplett gelîscht !      }
    gotoxy(1,_scry);
    case wnum of
      1:        { PullDown-MenÅs }
      P_OFFS+0: { PopUp-MenÅs    }
    end;
  end;