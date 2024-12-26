#!/usr/bin/env dss.sh
var ds =
  Packages.com.ti.ccstudio.scripting.environment.ScriptingEnvironment.instance().getServer(
    "DebugServer.1"
  );
ds.setConfig("targetConfigs/TMS320DM6467.ccxml");
var arm926 = ds.openSession(
  "Texas Instruments XDS100v3 USB Debug Probe/ARM926"
);
arm926.expression.evaluate('GEL_LoadGel("assets/gel/davincihd_arm.gel")');
arm926.target.connect();
var c64xp = ds.openSession("Texas Instruments XDS100v3 USB Debug Probe/C64XP");
c64xp.expression.evaluate('GEL_LoadGel("assets/gel/davincihd_dsp.gel")');
c64xp.target.connect();
c64xp.memory.loadProgram("x264.out", ["x264.out"].concat(arguments));
c64xp.clock.runBenchmark();
