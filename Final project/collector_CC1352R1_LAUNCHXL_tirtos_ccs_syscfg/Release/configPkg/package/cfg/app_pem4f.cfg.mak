# invoke SourceDir generated makefile for app.pem4f
app.pem4f: .libraries,app.pem4f
.libraries,app.pem4f: package/cfg/app_pem4f.xdl
	$(MAKE) -f D:\workspace_v9_2\collector_CC1352R1_LAUNCHXL_tirtos_ccs_syscfg/src/makefile.libs

clean::
	$(MAKE) -f D:\workspace_v9_2\collector_CC1352R1_LAUNCHXL_tirtos_ccs_syscfg/src/makefile.libs clean

