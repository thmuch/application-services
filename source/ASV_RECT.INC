{ ASV 1.5 RECT vom 20.12.1991 }

function rc_equal(src,dst : JREC) : boolean;

  begin
    if src.all=dst.all then rc_equal:=true
      else rc_equal:=false;
  end;


function rc_inside(x,y : byte; r : JREC) : boolean;

  begin
    if (x>=r.xl) and (x<=r.xr) and (y>=r.yo) and (y<=r.yu) then rc_inside:=true
      else rc_inside:=false;
  end;


function rc_intersect(src : JREC; var dst : JREC) : boolean;

  begin
    if src.all<>dst.all then
      begin
        if (sgn(dst.xl-src.xr)<>sgn(dst.xr-src.xl)) and
           (sgn(dst.yo-src.yu)<>sgn(dst.yu-src.yo)) then rc_intersect:=true
        else rc_intersect:=false;
      end
    else rc_intersect:=true;
    dst.all:=0;
  end;


procedure rc_copy(src : JREC; var dst : JREC);
  var xbis,ybis             : integer;
      q,w,brt               : byte;
      ofss,ofsd,ofss2,ofsd2 : word;

  begin
    if (src.xl>0) and (src.xr<=anzcol) and (src.xl<=src.xr) and
       (src.yo>0) and (src.yu<=anzline) and (src.yo<=src.yu) then
      begin
        xbis:=dst.xl+(src.xr-src.xl);
        ybis:=dst.yo+(src.yu-src.yo);
        if dst.xl=0 then
          begin
            dst.xl:=1;
            inc(src.xl);
          end;
        if dst.yo=0 then
          begin
            dst.yo:=1;
            inc(src.yo);
          end;
        if xbis>anzcol then
          begin
            dec(src.xr,xbis-anzcol);
            xbis:=anzcol;
          end;
        if ybis>anzline then
          begin
            dec(src.yu,ybis-anzline);
            ybis:=anzline;
          end;
        if (dst.xl<=xbis) and (dst.yo<=ybis) then
          begin
            dst.xr:=xbis;
            dst.yu:=ybis;
            if src.pos=dst.pos then exit;
            brt:=src.xr-src.xl;
            if dst.yo<src.yo then
              begin
                ofss:=pred(src.yo)*loff+(pred(src.xl) shl 1);
                ofsd:=pred(dst.yo)*loff+(pred(dst.xl) shl 1);
                for q:=0 to (src.yu-src.yo) do
                  begin
                    ofss2:=ofss;
                    ofsd2:=ofsd;
                    for w:=0 to brt do
                      begin
                        memw[vioseg:ofsd2]:=memw[vioseg:ofss2];
                        inc(ofss2,2);
                        inc(ofsd2,2);
                      end;
                    inc(ofss,loff);
                    inc(ofsd,loff);
                  end;
              end
            else if (dst.yo>src.yo) or (dst.xl>src.xl) then
              begin
                ofss:=pred(src.yu)*loff+(pred(src.xr) shl 1);
                ofsd:=pred(dst.yu)*loff+(pred(dst.xr) shl 1);
                for q:=0 to (src.yu-src.yo) do
                  begin
                    ofss2:=ofss;
                    ofsd2:=ofsd;
                    for w:=0 to brt do
                      begin
                        memw[vioseg:ofsd2]:=memw[vioseg:ofss2];
                        dec(ofss2,2);
                        dec(ofsd2,2);
                      end;
                    dec(ofss,loff);
                    dec(ofsd,loff);
                  end;
              end
            else
              begin
                ofss:=pred(src.yu)*loff+(pred(src.xl) shl 1);
                ofsd:=pred(dst.yu)*loff+(pred(dst.xl) shl 1);
                for q:=0 to (src.yu-src.yo) do
                  begin
                    ofss2:=ofss;
                    ofsd2:=ofsd;
                    for w:=0 to brt do
                      begin
                        memw[vioseg:ofsd2]:=memw[vioseg:ofss2];
                        inc(ofss2,2);
                        inc(ofsd2,2);
                      end;
                    dec(ofss,loff);
                    dec(ofsd,loff);
                  end;
              end;
          end
        else dst.size:=0;
      end
    else dst.size:=0;
  end;