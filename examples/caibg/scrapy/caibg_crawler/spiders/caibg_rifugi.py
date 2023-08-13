"""CaiBgRifugiCrawler class inherits from CrawlSpider"""

import os
import re
from urllib.parse import urljoin
from urllib.request import urlopen

from scrapy.linkextractors import LinkExtractor
from scrapy.spiders import CrawlSpider, Rule
from w3lib.url import url_query_cleaner


def process_links(links):
    """Clean URL arguments leaving only those passed in the parameterlist keeping order"""
    for link in links:
        link.url = url_query_cleaner(link.url)
        yield link


class CaiBgRifugiCrawler(CrawlSpider):
    name = "caibg_rifugi"
    allowed_domains = ["www.caibergamo.it"]
    start_urls = [
        "https://www.caibergamo.it/geoportale/rifugi-bivacchi",
        "https://www.caibergamo.it/geoportale/rifugi-bivacchi?q=/geoportale/rifugi-bivacchi&title=&field_zona_orobie_target_id=All&field_proprieta_value=CAIBG&field_tipo_rifugio_value=All&page=0",
        "https://www.caibergamo.it/geoportale/rifugi-bivacchi?q=/geoportale/rifugi-bivacchi&title=&field_zona_orobie_target_id=All&field_proprieta_value=CAIBG&field_tipo_rifugio_value=All&page=1",
    ]
    rules = (
        Rule(
            LinkExtractor(
                allow=(
                    re.escape("https://www.caibergamo.it/geoportale/rifugi-bivacchi/"),
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
            "name": os.path.basename(response.url).replace("-", " ").title(),
            "zone": response.xpath(
                "//div[@class='field field--name-field-zona-orobie field--type-entity-reference field--label-hidden field__items']//a/text()"
            ).get(),
            "urls": [
                urlopen(urljoin(response.url, path)).geturl()
                for path in response.xpath(
                    "//div[@class='field field--name-field-accesso field--type-entity-reference field--label-hidden field__items']//a/@href"
                ).extract()
            ],
        }
