"""CaiBgSentieriCrawler class inherits from CrawlSpider"""

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


class CaiBgSentieriCrawler(CrawlSpider):
    name = "caibg_sentieri"
    allowed_domains = ["www.caibergamo.it"]
    start_urls = [
        "https://www.caibergamo.it/geoportale/sentieri",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=0",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=1",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=2",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=3",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=4",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=5",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=6",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=7",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=8",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=9",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=10",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=11",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=12",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=13",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=14",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=15",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=16",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=17",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=18",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=19",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=20",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=21",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=22",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=23",
        "https://www.caibergamo.it/geoportale/sentieri?q=/geoportale/sentieri&title=&field_zona_orobie_target_id=All&field_difficolta_escursionistica_target_id=All&page=24",
    ]
    rules = (
        Rule(
            LinkExtractor(
                allow=(re.escape("https://www.caibergamo.it/geoportale/sentieri/"),)
            ),
            process_links=process_links,
            callback="parse_item",
            follow=True,
        ),
    )

    def parse_item(self, response):
        return {
            "url": response.url,
            "name": response.xpath(".//title/text()").get().split(" | ")[0],
            "zone": response.xpath(
                "//div[@class='field field--name-field-zona-orobie field--type-entity-reference field--label-inline clearfix']//a/text()"
            ).get(),
            "urls": [
                urlopen(urljoin(response.url, path))
                .geturl()
                .replace("/en/", "/")
                .replace("/de/", "/")
                for path in response.xpath(
                    "//div[@class='field field--name-field-sentieri-vicini field--type-entity-reference field--label-hidden field__items']//a/@href"
                ).extract()
            ],
        }
