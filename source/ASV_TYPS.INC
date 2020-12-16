{ ASV 1.4 TYPS vom 13.06.1991 }

  type vtype = record
                 case integer of
                   0  : (b : byte);
                   1  : (si : shortint);
                   2  : (i : integer);
                   3  : (w : word);
                   4  : (l : longint);
                   5  : (bo : boolean);
                   6  : (c : char);
                   7  : (s : string);
                   8  : (r : real);
                   9  : (sr : single);
                   10 : (d : double);
                   11 : (e : extended);
                   12 : (co : comp);
               end;
       pvtype   = procedure(wnum : byte);
       dobjtype = procedure(wnum,com,dat1,dat2 : byte);
       wordptr  = ^word;
       byteptr  = ^byte;