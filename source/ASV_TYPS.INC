{ ASV 1.5 TYPS vom 21.12.1991 }

  type vtype    = record
                    case byte of
                       0: (b  : byte);
                       1: (si : shortint);
                       2: (i  : integer);
                       3: (w  : word);
                       4: (l  : longint);
                       5: (bo : boolean);
                       6: (c  : char);
                       7: (s  : string);
                       8: (r  : real);
                       9: (sr : single);
                      10: (d  : double);
                      11: (e  : extended);
                      12: (co : comp);
                  end;
       JREC     = record
                    case byte of
                      0: (all         : longint);
                      1: (pos,size    : word);
                      2: (xl,yo,xr,yu : byte);
                  end;
       dbtype   = array [0.._dbutmax] of
                    record
                      stat,x,y,frame,fcol,bexit : byte;
                      amenu,kashft,kacode,tlen  : byte;
                      name                      : string[50];
                    end;
       dobjtype = procedure(wnum,com,dat1,dat2 : byte);
       pvtype   = procedure(wnum : byte);
       evnttype = procedure(job,stat : byte);
       WORDPTR  = ^word;
       BYTEPTR  = ^byte;
       jobtype  = record
                    stat,call,flag : byte;
                    sec            : word;
                    proc           : evnttype;
                    lastcall       : longint;
                  end;