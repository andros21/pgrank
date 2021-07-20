"""CaiBgRifugiCrawler class inherits from CrawlSpieder"""

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


class CaiBgRifugiCrawler(CrawlSpider):
    name = "caibg_rifugi"
    allowed_domains = ["geoportale.caibergamo.it"]
    start_urls = [
        "http://geoportale.caibergamo.it/it/rifugi-e-bivacchi/",
        "http://geoportale.caibergamo.it/it/rifugi-e-bivacchi?page=1/",
        "http://geoportale.caibergamo.it/it/rifugi-e-bivacchi?page=2/",
    ]
    rules = (
        Rule(
            LinkExtractor(
                allow=(re.escape("http://geoportale.caibergamo.it/it/rifugio/"),)
            ),
            process_links=process_links,
            callback="parse_item",
            follow=True,
        ),
    )

    def parse_item(self, response):
        return {
            "url": response.url,
            "name": response.xpath("//div[@class='section']//h1/text()")[0].get(),
            "zone": response.xpath(
                "//div[@class='field field-name-field-zona-orobie field-type-taxonomy-term-reference field-label-inline clearfix']//a/text()"
            ).get(),
            "urls": [
                urllib.request.urlopen(urljoin(response.url, path)).geturl()
                for path in response.xpath(
                    "//div[@class='field field-name-field-accesso field-type-node-reference field-label-hidden']//a/@href"
                ).extract()
            ],
        }
