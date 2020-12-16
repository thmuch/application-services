{**************************************
 *   ASV - Application Services 1.5   *
 * Copyright (c)1990-91 by Softdesign *
 *      written by Thomas Much        *
 **************************************
 *  entwickelt am:      24.05.1990    *
 *  letztes Update am:  21.12.1991    *
 **************************************}

unit asv;
{$A+,B-,D-,E-,F+,I-,L-,N-,O+,R-,S-,V+}

interface

  uses dos,crt;

  const _magic     = 18011972;
        _asv       = 1.5;      _wmax      = 99;     _dbutmax  = 19;
        _bmax      = 29;       _imax      = 14;     _womax    = 4;
        M_EXIT     = 255;      _jobmax    = 9;      _dobjmax  = 19;
        M_OK       = 0;        M_HELP     = 254;
        M_NOP      = -1;       H_PULL     = 0;      B_OFF     = 0;
        M_CANCEL   = -2;       H_POP      = 1;      B_ON      = 1;
        M_ERROR    = -10;      H_DIAL     = 2;      B_NXON    = 2;
        E_NFOUND   = -11;      M_DIS      = 253;    B_NXOFF   = 3;
        E_NREENT   = -12;      KEY_NONE   = 0;      B_DISABLED= 255;
        E_NDRAW    = -13;      KEY_NORM   = 127;    B_ENABLED = 0;
        E_MOUTSCR  = -14;      KEY_SPEC   = 27;
        E_MUSED    = -15;      M_PULL     = 0;      D_NONE    = 0;
        E_NINIT    = -16;      M_DROP     = 1;      D_CLOSE   = 1;
        E_NBUT     = -17;      M_NMOVE    = 128;    D_MOVE    = 2;
        E_DSTRUC   = -18;      X_BOTH     = 0;      D_GRAF    = 4;
        E_DTBIG    = -19;      X_OK       = 1;      F_OK      = 0;
        E_MSTRUC   = -20;      X_CANCEL   = 2;      F_ERROR   = -1;
        RSHIFT     = 1;        LSHIFT     = 2;      F_NONE    = 255;
        CTRL       = 4;        ALT        = 8;
        SCRLLOCK   = 16;       NUMLOCK    = 32;     P_TIMEOUT = 1;
        CAPSLOCK   = 64;       INS        = 128;    P_EXMIT   = 8;
        L_USA      = 1;        PC         = 0;      P_ONLINE  = 16;
        L_FRG      = 2;        XT         = 1;      P_EPAPER  = 32;
        L_GB       = 8;        AT         = 3;      J_OK      = 0;
        J_ENABLED  = 254;      J_DISABLED = 0;      J_ERROR   = -1;
        S_DESK     = 1;        S_MENU     = 2;      S_DIALOG  = 4;
        S_UNKNOWN  = 0;        S_ALL      = 0;
        S_SMALL    = 0;        S_MID      = 1;      S_WIDE    = 2;
        A_NCHECKED = 1;        C_SPACE    = 32;     S_LRIGHT  = 128;
        A_ENABLED  = 3;        C_WDOT     = 176;
        A_CHECKED  = 2;        C_MDOT     = 177;    P_OFFS   = 123;
        A_DISABLED = 4;        C_CDOT     = 178;
        B_FNONE    = 0;        B_FSINGLE  = 1;      B_FHALF  = 2;
        B_FDOUBLE  = 3;        B_CLICK    = 0;      B_KNONE  = 0;
        B_OFFS     = 200;      B_TOUCH    = 1;      B_MNONE  = 255;
        B_ALL      = 255;      B_ERROR    = -1;     B_OK     = 0;
        M_LEFT     = 1;        M_MID      = 4;      M_RIGHT  = 2;
        K_F1       = 59;       K_F2       = 60;     K_INS    = 82;
        K_F3       = 61;       K_F4       = 62;     K_DEL    = 83;
        K_F5       = 63;       K_F6       = 64;     K_HOME   = 71;
        K_F7       = 65;       K_F8       = 66;     K_END    = 79;
        K_F9       = 67;       K_F10      = 68;     K_PGUP   = 73;
        K_ESC      = 27;       K_TAB      = 9;      K_PGDN   = 81;
        K_BS       = 8;        K_RET      = 13;     K_UP     = 72;
        K_LF       = 10;       K_FF       = 12;     K_DOWN   = 80;
                               K_LEFT     = 75;     K_RIGHT  = 77;


  {$I c:\pascal\units\asv_inc\asv_typs.inc}


  var work    : array [0.._wmax] of string[90];
      intout  : array [0.._imax] of integer;
      byteout : array [0.._bmax] of byte;
      wordout : array [0.._womax] of word;
      regs    : registers;
      void    : vtype;
      JRECT   : JREC;

  procedure hide;
  procedure hidecursor;
  procedure showcursor;
  function left(teil : string; anz : byte) : string;
  function right(teil : string; anz : byte) : string;
  function space(anz : byte) : string;
  function strg(teil : string; anz : byte) : string;
  procedure cutl(var teil : string);
  procedure cutr(var teil : string);
  function trimf(teil : string) : string;
  procedure trim(var teil : string);
  function upper(teil : string) : string;
  function lower(teil : string) : string;
  function sgn(wert : longint) : shortint;
  function ftoa(zahl : real) : string;
  function atof(zeichen : string) : real;
  function itoa(zahl : longint) : string;
  function atoi(zeichen : string) : longint;
  procedure hardcopy;
  procedure printxy(x,y : byte; txt : string);
  function getvmemp(var p : pointer) : boolean;
  function prn_stat(bits : byte) : boolean;
  function kbshift(bits : byte) : boolean;
  procedure kbset(bits : byte);
  procedure kbclr(bits : byte);
  procedure showmouse;
  procedure hidemouse;
  function mousex : byte;
  function mousey : byte;
  function mouseb : byte;
  procedure getmouse(var mx,my,mb : byte);
  procedure setmouse(mx,my : byte);
  procedure defmouse(smask,cmask : word);
  function attr(ink,pap : byte) : byte;
  function exist(path : string) : boolean;
  procedure clsp(att,zch : byte);
  procedure cls(att : byte);
  procedure setattr(tx,ty,art,len : byte);
  function form_objins(nobj : dobjtype) : integer;
  function form_objdel(objn : byte) : integer;
  function form_objreplace(objn : byte; nobj : dobjtype) : integer;
  procedure form_objcall(objn,wnum,com,dat1,dat2 : byte);
  function dtext(dx,dy : byte; dt : string) : string;
  function dframe(dx,dy,db,dh : byte; dt : string) : string;
  function dpush(dx,dy,ds : byte; title : string) : string;
  function dradio(dx,dy,dn,ds : byte; title : string) : string;
  function dalpha(dx,dy,db,dmb,ds : byte;title,cont:string):string;
  procedure form_setpb(wnum,ds : byte);
  function form_getpb(wnum : byte) : byte;
  procedure form_setrb(wnum,ds : byte);
  function form_getrb(wnum : byte) : byte;
  function form_findrb(von,bis,dn : byte) : byte;
  procedure form_setan(wnum : byte; cont : string);
  function form_getan(wnum : byte) : string;
  procedure form_setans(wnum,ds : byte);
  function form_getans(wnum : byte) : byte;
  function rc_equal(src,dst : JREC) : boolean;
  function rc_inside(x,y : byte; r : JREC) : boolean;
  function rc_intersect(src : JREC; var dst : JREC) : boolean;
  procedure rc_copy(src : JREC; var dst : JREC);
  procedure dbut_draw(was : byte);
  function dbut_insert(x,y,frm,fcol,ext,menu,kshft,kcode : byte; name : string) : integer;
  function dbut_delete(num : byte) : integer;
  procedure dbut_update(num,frm,fcol,menu,kshft,kcode : byte; name : string);
  function job_add(proc: evnttype; sec : word; cstat:byte):integer;
  function job_kill(num : byte) : integer;
  procedure job_setstat(num,stat : byte);
  function job_getstat(num : byte) : integer;
  procedure job_settime(num : byte; sec : word);
  function job_gettime(num : byte) : longint;
  procedure job_setcall(num,cstat : byte);
  function job_getcall(num : byte) : integer;
  procedure job_disable;
  procedure job_enable;
  procedure menu_event(stat : byte);
  function _dactive : byte;
  function dialog : integer;
  function form_alert(title,msg,but : string; def : byte) :integer;
  procedure form_errexit(stat : byte);
  procedure menu_sets(stat : byte);
  procedure form_error(num : integer);
  procedure form_seterrl(cc : integer);
  procedure form_setattr(bits : word);
  procedure menu_init;
  procedure menu_draw;
  function menu_watch : integer;
  procedure menu_kill;
  procedure menu_add(wrk : string);
  procedure menu_delete(num : byte);
  procedure menu_rename(num : byte; wrk : string);
  function menu_pop(wo : byte) : integer;
  procedure form_extcol(sdia,hdia,sale,hale,spop,hpop,dpop : byte);
  procedure menu_setc(was,saw,has,haw,dis,shdw : byte);
  procedure menu_resetc;
  procedure menu_setschr(ccode : byte);
  procedure menu_space(attr : byte);
  procedure scrforce(anzx,anzy : byte; avcol : boolean);
  procedure menu_setp(procv : pvtype);
  procedure menu_resetp;
  procedure menu_attr(num,attrs : byte);
  function menu_stat(num : byte) : byte;
  function menu_text(num : byte) : string;
  procedure menu_inquire;
  function _scrx : byte;
  function _scry : byte;
  function color : boolean;
  function mouse : boolean;
  function mvisible : boolean;
  procedure mouse_sets(sx,sy : integer);
  procedure mouse_resets;
  procedure mouse_sett(dst : integer);
  procedure mouse_resett;
  function asverror : integer;
  procedure asv_init;
  procedure form_setslsdelay(dl : word);
  function form_getslsdelay : word;
  procedure form_setslmdelay(dl : word);
  function form_getslmdelay : word;
  function form_getslidermax(wnum : byte) : word;
  procedure form_setslider(wnum : byte; wert : word);
  function form_getslider(wnum : byte) : word;
  procedure form_setslidermax(wnum : byte; max : word);
  function dhslider(x,y,brt : byte; wert,max : word) : string;
  function dvslider(x,y,hhe : byte; wert,max : word) : string;
  function _hslider : integer;
  function _vslider : integer;
  function sts(var strng : string; ch : char) : string;
  function stn(var strng : string; ch : char) : integer;
  function fileselect(title,path,exts,default : string) : string;
  function colours(title : string; attr : byte) : integer;


implementation

  const _mmax    = 19;  _emin  = -20; _viormax = 2239; fmax = 399;
        J_UNUSED = 255; J_FLAG = 123; B_UNUSED = 0;    B_USED = 255;

  var mbegin,mxpos                : array [0.._mmax] of integer;
      mecurr                      : array [0.._mmax] of byte;
      mhkey                       : array [0.._wmax] of char;
      mhpos,attrib                : array [0.._wmax] of byte;
      mwork                       : array [0.._wmax] of string[30];
      dwork                       : array [0.._wmax] of string[90];
      bxpos                       : array [0..19] of byte;
      max_entry,m_max,m_stat      : integer;
      anzline,anzcol,asvierror    : integer;
      ccode,menn,loff,m_num       : integer;
      curr_sx,curr_sy,curr_dst    : integer;
      oanz,fanf,fanz,exanz        : integer;
      hslnum,vslnum,lnum          : integer;
      dialog_aktiv,mausda,mausvis : boolean;
      farbe,menk,tstat            : boolean;
      vioseg,dbits,vofs,wq        : word;
      sldelay,slmdelay            : word;
      vioram                      : array [0.._viormax] of word;
      titleram                    : array [0..169] of word;
      weissaufschwarz,disabled,sc : byte;
      hellaufschwarz,shadow,aktiv : byte;
      schwarzaufweiss,meny,dhkpos : byte;
      manzbut,menx,sel_char,rbv   : byte;
      saw_dia,haw_dia,saw_ale,rbb : byte;
      saw_pop,haw_pop,dis_pop,ano : byte;
      do_rnum,do_pnum,do_anum,anp : byte;
      hellaufweiss,crsof,haw_ale  : byte;
      eexit_stat,mpd_stat,msp_atr : byte;
      fox,foy,oxp,oyp,mim_num     : byte;
      mpm_stat,clx,cly,dbutak     : byte;
      m_pcall                     : pvtype;
      dhkstr                      : string[30];
      sdummy,pfad                 : string;
      exitsave,heapsave,oldbreak  : pointer;
      p1,p2,p3                    : WORDPTR;
      dobjmem                     : array[0.._dobjmax] of dobjtype;
      files                       : array [0..fmax] of string[12];
      extension                   : array [0..4] of string[12];
      dbuttons                    : dbtype;
      jobs                        : array [0.._jobmax] of jobtype;


  {$I c:\pascal\units\asv_inc\asv_prcs.inc}

  {$I c:\pascal\units\asv_inc\asv_rect.inc}

  {$I c:\pascal\units\asv_inc\asv_dbut.inc}

  {$I c:\pascal\units\asv_inc\asv_jobs.inc}

  {$I c:\pascal\units\asv_inc\asv_dial.inc}

  {$I c:\pascal\units\asv_inc\asv_alrt.inc}

  {$I c:\pascal\units\asv_inc\asv_menu.inc}


procedure menu_inquire;

  begin
    byteout[0]:=weissaufschwarz;
    byteout[1]:=schwarzaufweiss;
    byteout[2]:=hellaufschwarz;
    byteout[3]:=hellaufweiss;
    byteout[4]:=disabled;
    byteout[5]:=shadow;
    byteout[6]:=mpd_stat;
    byteout[7]:=eexit_stat;
    byteout[8]:=get_fobjmax;
    byteout[9]:=sel_char;
    byteout[10]:=manzbut;
    byteout[11]:=PC;
    byteout[12]:=21;
    byteout[13]:=12;
    byteout[14]:=91;
    byteout[15]:=saw_dia;
    byteout[16]:=haw_dia;
    byteout[17]:=saw_ale;
    byteout[18]:=haw_ale;
    byteout[19]:=saw_pop;
    byteout[20]:=haw_pop;
    byteout[21]:=dis_pop;
    byteout[22]:=do_pnum;
    byteout[23]:=do_rnum;
    byteout[24]:=do_anum;
    byteout[25]:=msp_atr;
    byteout[26]:=0;
    byteout[27]:=0;
    byteout[28]:=0;
    byteout[29]:=0;
    intout[0]:=ccode;
    intout[1]:=L_USA+L_FRG+L_GB;
    intout[2]:=_emin;
    intr($12,regs);
    intout[3]:=regs.ax;
    intout[4]:=0;
    intout[5]:=_viormax shl 1;
    intout[6]:=asvierror;
    intout[7]:=pred(max_entry);
    intout[8]:=curr_sx;
    intout[9]:=curr_sy;
    intout[10]:=curr_dst;
    intout[11]:=0;
    intout[12]:=0;
    intout[13]:=0;
    intout[14]:=0;
    wordout[0]:=dbits;
    wordout[1]:=vioseg;
    wordout[2]:=loff;
    wordout[3]:=0;
    wordout[4]:=0;
  end;


  {$I c:\pascal\units\asv_inc\asv_endp.inc}

  {$I c:\pascal\units\asv_inc\asv_objc.inc}

  {$I c:\pascal\units\asv_inc\asv_colr.inc}


procedure init;

  begin
    heapsave:=heaperror;
    heaperror:=@heaperr;
    exitsave:=exitproc;
    exitproc:=@asvexit;
    getintvec($23,oldbreak);
    m_stat:=-1;
    dbutak:=255;
    JRECT.all:=0;
    dialog_aktiv:=false;
    for wq:=0 to _dobjmax do dobjmem[wq]:=dobj_nothing;
    for wq:=0 to _jobmax do jobs[wq].stat:=J_UNUSED;
    for wq:=0 to _dbutmax do dbuttons[wq].stat:=B_UNUSED;
    do_pnum:=form_objins(pushbutton);
    do_rnum:=form_objins(radiobutton);
    do_anum:=form_objins(alphainp);
    form_seterrl(L_FRG);
    form_errexit(X_BOTH);
    form_setattr(D_CLOSE+D_MOVE+D_GRAF);
    menu_sets(M_DROP);
    menu_resetp;
    menu_setschr(7);
    menu_space(S_SMALL);
  end;


begin
  init;
  asv_init;
  objc_init;
end.