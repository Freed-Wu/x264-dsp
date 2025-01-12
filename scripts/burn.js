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
  'GEL_LoadGel("targetConfigs/davincihd1080p_arm.gel")'
);
arm926.target.connect();
var c64xp = ds.openSession("Texas Instruments XDS100v3 USB Debug Probe/C64XP");
c64xp.expression.evaluate(
  'GEL_LoadGel("/opt/ccstudio/ccs/ccs_base/emulation/boards/evmdm6467/gel/davincihd1080p_dsp.gel")'
);
c64xp.target.connect();

if (arguments[0] && arguments[0] !== "--") {
  var ddr2 = 0xa0000000;
  arm926.memory.loadRaw(0, ddr2, arguments[0], 32, false); // DDR2 SDRAM
  arm926.memory.verifyBinaryProgram(arguments[0], ddr2);
}

var args = [];
for (i in arguments)
  if (arguments[i] === "--") {
    args = arguments.slice(Number(i) + 1);
    break;
  }
if (args.length) {
  c64xp.memory.loadProgram(args[0], args);
  c64xp.memory.verifyProgram(args[0]);
  c64xp.clock.runBenchmark();
}
