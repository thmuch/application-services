{$F+}
procedure hilfe(wnum : byte); {$F-}

  begin
    gotoxy(1,_scry);
    write(space(_scrx));
    { M_EXIT braucht nicht abgefragt zu werden, bei jedem unbekannten
      Wert wird die unterste Bildschirmzeile komplett gel�scht !      }
    gotoxy(1,_scry);
    case wnum of
      1:        { PullDown-Men�s }
      P_OFFS+0: { PopUp-Men�s    }
    end;
  end;