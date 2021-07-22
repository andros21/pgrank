"""CaiBgSentieriCrawler class inherits from CrawlSpider"""

import re
import urllib.request

from scrapy.linkextractors import LinkExtractor
from scrapy.spiders import CrawlSpider, Rule
from w3lib.url import url_query_cleaner, urljoin


def process_links(links):
    """Clean URL arguments leaving only those passed in the parameterlist keeping order"""
    for link in links:
        link.url = url_query_cleaner(link.url)
        yield link


class CaiBgSentieriCrawler(CrawlSpider):
    name = "caibg_sentieri"
    allowed_domains = ["geoportale.caibergamo.it"]
    start_urls = [
        "http://geoportale.caibergamo.it/it/sentieri",
        "http://geoportale.caibergamo.it/it/sentieri?page=1",
        "http://geoportale.caibergamo.it/it/sentieri?page=2",
        "http://geoportale.caibergamo.it/it/sentieri?page=3",
        "http://geoportale.caibergamo.it/it/sentieri?page=4",
        "http://geoportale.caibergamo.it/it/sentieri?page=5",
        "http://geoportale.caibergamo.it/it/sentieri?page=6",
        "http://geoportale.caibergamo.it/it/sentieri?page=7",
        "http://geoportale.caibergamo.it/it/sentieri?page=8",
        "http://geoportale.caibergamo.it/it/sentieri?page=9",
        "http://geoportale.caibergamo.it/it/sentieri?page=10",
        "http://geoportale.caibergamo.it/it/sentieri?page=11",
        "http://geoportale.caibergamo.it/it/sentieri?page=12",
        "http://geoportale.caibergamo.it/it/sentieri?page=13",
        "http://geoportale.caibergamo.it/it/sentieri?page=14",
        "http://geoportale.caibergamo.it/it/sentieri?page=15",
        "http://geoportale.caibergamo.it/it/sentieri?page=16",
        "http://geoportale.caibergamo.it/it/sentieri?page=17",
    ]
    rules = (
        Rule(
            LinkExtractor(
                allow=(
                    re.escape("http://geoportale.caibergamo.it/it/sentiero/"),
                    re.escape("http://geoportale.caibergamo.it/it/node/"),
                )
            ),
            process_links=process_links,
            callback="parse_item",
            follow=True,
        ),
    )

    def parse_item(self, response):
        return {
            "url": response.url,
            "name": response.xpath("//div[@class='section']//h1/text()").get(),
            "zone": response.xpath(
                "//div[@class='field field-name-field-zona-orobie field-type-taxonomy-term-reference field-label-inline clearfix']//a/text()"
            ).get(),
            "urls": [
                urllib.request.urlopen(urljoin(response.url, path)).geturl()
                for path in response.xpath(
                    "//div[@class='field field-name-field-sentieri-vicini field-type-node-reference field-label-hidden']//a/@href"
                ).extract()
            ],
        }
