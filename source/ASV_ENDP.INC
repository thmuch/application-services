{ ASV 1.3 ENDP vom 20.04.1991 }

function _scrx : byte;

  begin
    _scrx:=anzcol;
  end;


function _scry : byte;

  begin
    _scry:=anzline;
  end;


function color : boolean;

  begin
    color:=farbe;
  end;


function mouse : boolean;

  begin
    mouse:=mausda;
  end;


function mvisible : boolean;

  begin
    mvisible:=mausvis;
  end;


procedure mouse_sets(sx,sy : integer);

  begin
    if mausda then
      begin
        if sx<1 then sx:=1;
        if sy<1 then sy:=1;
        regs.ax:=15;
        regs.cx:=sx;
        regs.dx:=sy;
        intr($33,regs);
        curr_sx:=sx;
        curr_sy:=sy;
      end;
  end;


procedure mouse_resets;

  begin
    mouse_sets(8,16);
  end;


procedure mouse_sett(dst : integer);

  begin
    if mausda then
      begin
        if dst<0 then dst:=0;
        regs.ax:=19;
        regs.dx:=dst;
        intr($33,regs);
        curr_dst:=dst;
      end;
  end;


procedure mouse_resett;

  begin
    mouse_sett(64);
  end;


function asverror : integer;

  begin
    asverror:=asvierror;
    asvierror:=0;
  end;


function heaperr(size : word) : integer;

  begin
    heaperr:=1;
  end;


procedure asvexit;

  begin
    exitproc:=exitsave;
    heaperror:=heapsave;
    if mausda then
      begin
        hidemouse;
        regs.ax:=0;
        intr($33,regs);
      end;
    setintvec($23,oldbreak);
  end;