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
arm926.expression.evaluate(
  'GEL_LoadGel("targetConfigs/gel/davincihd1080p_arm.gel")'
);
arm926.target.connect();
// after OnTargetConnect()
arm926.expression.evaluate("DSP_Boot_from_DDR2()");
var c64xp = ds.openSession("Texas Instruments XDS100v3 USB Debug Probe/C64XP");
c64xp.expression.evaluate(
  'GEL_LoadGel("/opt/ccstudio/ccs/ccs_base/emulation/boards/evmdm6467/gel/davincihd1080p_dsp.gel")'
);
c64xp.target.connect();
var bin = "build/x264.out";
var args = [];
for (i in arguments)
  if (arguments[i] === "--") {
    args = [bin].concat(arguments.slice(Number(i) + 1));
    break;
  }
c64xp.memory.loadProgram(bin, args);
c64xp.memory.verifyProgram(bin);
if (arguments[0] && arguments[0] !== "--") {
  var ddr2 = 0xa0000000;
  arm926.memory.loadRaw(0, ddr2, arguments[0], 32, false); // DDR2 SDRAM
  arm926.memory.verifyBinaryProgram(arguments[0], ddr2);
}
if (args.length) c64xp.clock.runBenchmark();
