for interval in 0.1 0.2 0.5 1.0 2.0 5.0; do
    ./../../ns3 run --no-build --cwd=$PWD \
    "aloha_vs_dcf --RngRun=1 --numOfStations=10 \
    --isDcf=false --collectPcap=false \
    --interval=${interval}s --outFileName=results/intensity-${interval}.txt"
done