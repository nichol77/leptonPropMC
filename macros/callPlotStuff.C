

void callPlotStuff() {

    gSystem->CompileMacro("interactions.cxx","k");
    gSystem->CompileMacro("plotStuff.C","k");
    plotStuff();

}
