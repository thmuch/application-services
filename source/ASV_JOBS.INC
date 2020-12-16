{ ASV 1.5 JOBS vom 19.12.1991 }

function job_add(proc: evnttype; sec : word; cstat : byte) : integer;
  var jnr,q : byte;

  begin
    jnr:=255;
    for q:=_jobmax downto 0 do
      if jobs[q].stat=J_UNUSED then jnr:=q;
    if jnr<255 then
      begin
        with jobs[jnr] do
          begin
            stat:=J_ENABLED;
            flag:=J_FLAG;
            call:=cstat;
            lastcall:=0;
          end;
        jobs[jnr].sec:=sec;
        jobs[jnr].proc:=proc;
        job_add:=jnr;
      end
    else job_add:=J_ERROR;
  end;


function job_kill(num : byte) : integer;

  begin
    if num<=_jobmax then
      begin
        if jobs[num].stat<>J_UNUSED then
          begin
            jobs[num].stat:=J_UNUSED;
            job_kill:=J_OK;
          end
        else job_kill:=J_ERROR;
      end
    else job_kill:=J_ERROR;
  end;


procedure job_setstat(num,stat : byte);

  begin
    if (num<=_jobmax) and (stat in [J_ENABLED,J_DISABLED]) then
      if jobs[num].stat<>J_UNUSED then jobs[num].stat:=stat;
  end;


function job_getstat(num : byte) : integer;

  begin
    if num<=_jobmax then
      begin
        if jobs[num].stat=J_UNUSED then job_getstat:=J_ERROR
          else job_getstat:=jobs[num].stat;
      end
    else job_getstat:=J_ERROR;
  end;


procedure job_settime(num : byte; sec : word);

  begin
    if (num<=_jobmax) then
      if jobs[num].stat<>J_UNUSED then jobs[num].sec:=sec;
  end;


function job_gettime(num : byte) : longint;

  begin
    if num<=_jobmax then
      begin
        if jobs[num].stat=J_UNUSED then job_gettime:=J_ERROR
          else job_gettime:=jobs[num].sec;
      end
    else job_gettime:=J_ERROR;
  end;


procedure job_setcall(num,cstat : byte);

  begin
    if (num<=_jobmax) then
      if jobs[num].stat<>J_UNUSED then jobs[num].call:=cstat;
  end;


function job_getcall(num : byte) : integer;

  begin
    if num<=_jobmax then
      begin
        if jobs[num].stat=J_UNUSED then job_getcall:=J_ERROR
          else job_getcall:=jobs[num].call;
      end
    else job_getcall:=J_ERROR;
  end;


procedure job_disable;
  var q : byte;

  begin
    for q:=0 to _jobmax do
      if jobs[q].stat<>J_UNUSED then
        if jobs[q].flag=J_FLAG then
          begin
            jobs[q].flag:=jobs[q].stat;
            jobs[q].stat:=J_DISABLED;
          end;
  end;


procedure job_enable;
  var q : byte;

  begin
    for q:=0 to _jobmax do
      if jobs[q].stat<>J_UNUSED then
        if jobs[q].flag<>J_FLAG then
          begin
            jobs[q].stat:=jobs[q].flag;
            jobs[q].flag:=J_FLAG;
          end;
  end;


procedure menu_event(stat : byte);
  var run             : array [0.._jobmax] of byte;
      runanz,q,jn     : byte;
      h,m,s,s1        : word;
      ptime,stime,dif : longint;

  begin
    gettime(h,m,s,s1);
    ptime:=s1+s*100+m*6000+h*360000;
    runanz:=0;
    for q:=0 to _jobmax do
      if jobs[q].stat=J_ENABLED then
        begin
          dif:=ptime-jobs[q].lastcall;
          if dif<0 then jobs[q].lastcall:=0
          else
            begin
              if dif>=jobs[q].sec then
                if (jobs[q].call=S_ALL) or ((jobs[q].call and stat)=stat) then
                  begin
                    run[runanz]:=q;
                    inc(runanz);
                  end;
            end;
        end;
    if runanz>0 then
      begin
        stime:=maxlongint;
        for q:=0 to pred(runanz) do
          if jobs[run[q]].lastcall<stime then
            begin
              jn:=run[q];
              stime:=jobs[jn].lastcall;
            end;
        jobs[jn].lastcall:=ptime;
        jobs[jn].proc(jn,stat);
      end;
    JRECT.all:=0;
  end;