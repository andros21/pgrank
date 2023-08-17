# :mountain: caibg

### Input data

Using [`scrapy`](https://scrapy.org/) web crawl capabilities with two custom [spiders](scrapy/caibg_crawler/spiders), have been crawled the sub domains [rifugi-bivacchi](https://www.caibergamo.it/geoportale/rifugi-bivacchi) and [sentieri](https://www.caibergamo.it/geoportale/sentieri) (prerequisites for running the above commands `scrapy` python package)

```shell
pushd scrapy
scrapy crawl caibg_rifugi   --nolog -O ../data/caibg-rifugi.json
scrapy crawl caibg_sentieri --nolog -O ../data/caibg-sentieri.json
popd
jq -s '.[0] + .[1]' data/caibg_rifugi.json data/caibg_sentieri.json \
   | jq '{"objects": .}' \
   > data/caibg.json
```

And glue together the results in `caibg.json`, a perfect graph of links between these two sub domains, ready to be parsed by `pgrank`

### Output data

```shell
pgrank data/caibg.json data/caibg.csv
```

The result is a `csv` file contains pageranks in the same order of the given input url nodes from `json` ... That's it. `pgrank` app only compute intensive tasks, future analysis can be delegated to more friendly framework, such as `pandas`

### Results

To make the results more readable `caibg.py` creates a summing up markdown table [`caibg.md`](caibg.md) (prerequisites for running `caibg.py` are `pandas` and `tabulate` python packages)
