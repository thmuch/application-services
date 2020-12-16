{ ASV 1.4 ENDP vom 13.06.1991 }

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
  end;


procedure asv_init;
  var adapfound : boolean;

  begin
    adapfound:=false;
    regs.ax:=$1a00;
    intr($10,regs);
    if regs.al=$1a then adapfound:=true
    else
      begin
        regs.ah:=$12;
        regs.bl:=$10;
        intr($10,regs);
        if regs.bl<>$10 then adapfound:=true;
      end;
    regs.ah:=15;
    intr($10,regs);
    if regs.al=7 then vioseg:=$b000 else vioseg:=$b800;
    farbe:=not((regs.al=0) or (regs.al=2) or (regs.al=7));
    anzcol:=regs.ah;
    loff:=anzcol shl 1;
    if not(adapfound) then anzline:=25
      else anzline:=byteptr(ptr($40,$84))^+1;
    regs.ah:=5;
    regs.al:=0;
    intr($10,regs);
    mausda:=false;
    manzbut:=0;
    regs.ax:=0;
    regs.bx:=0;
    intr($33,regs);
    if regs.ax=$ffff then
      begin
        mausda:=true;
        manzbut:=regs.bx;
        regs.ax:=29;
        regs.bx:=0;
        intr($33,regs);
      end;
    mausvis:=false;
    asvierror:=0;
    menu_resetc;
    showmouse;
    defmouse($ffff,$7700);
    mouse_resets;
    mouse_resett;
    menu_inquire;
  end;