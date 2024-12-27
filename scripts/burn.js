#!/usr/bin/env dss.sh
var ds =
  Packages.com.ti.ccstudio.scripting.environment.ScriptingEnvironment.instance().getServer(
    "DebugServer.1"
  );
ds.setConfig("targetConfigs/TMS320DM6467.ccxml");
var arm926 = ds.openSession(
  "Texas Instruments XDS100v3 USB Debug Probe/ARM926"
);
// /opt/ccstudio/ccs/ccs_base/emulation/boards/evmdm6467/gel/davincihd1080p_arm.gel
// comment Setup_Pll0_990_MHz_OscIn()
arm926.expression.evaluate('GEL_LoadGel("assets/gel/davincihd1080p_arm.gel")');
arm926.target.connect();
// after OnTargetConnect()
arm926.expression.evaluate('DSP_Boot_from_DDR2()');
var c64xp = ds.openSession("Texas Instruments XDS100v3 USB Debug Probe/C64XP");
c64xp.expression.evaluate('GEL_LoadGel("/opt/ccstudio/ccs/ccs_base/emulation/boards/evmdm6467/gel/davincihd1080p_dsp.gel")');
c64xp.target.connect();
c64xp.memory.loadProgram("x264.out", ["x264.out"].concat(arguments));
c64xp.clock.runBenchmark();
