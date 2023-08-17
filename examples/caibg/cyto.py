#!/usr/bin/env python
"""Create a cytoscape rappresentation of caibg network"""

import json
import re
import sys

import networkx as nx
import pandas as pd

cmap = {
    "Val di Scalve": "#D95562",
    "Val Vertova": "#7D93B2",
    "Val San Martino": "#6090CF",
    "Monte Bronzone - Lago d'Iseo": "#6F5F78",
    "Valle Imagna": "#6C839B",
    "Bassa e Media Valle Brembana": "#27212D",
    "Val Serina": "#51B698",
    None: "#667461",
    "Valcanale": "#E0AC82",
    "Bassa e Media Val Seriana": "#9F3251",
    "Val Cavallina": "#413F53",
    "Alta Val Seriana": "#9AAAB1",
    "Valle Brembana": "#30A8CC",
    "Alta Valle Brembana": "#F2BC53",
    "Valle del Riso": "#4A7296",
    "Val Brembilla": "#7E82A5",
    "Val Taleggio": "#A7A5A8",
    "Val Seriana": "#78A6E7",
    "Val Gandino": "#F6F6F2",
}


def _metadata(cyto_json: dict, caibg: dict, rank: pd.DataFrame, color: dict) -> dict:
    """add metadata to cytoscape json dictionary"""
    min_rank = rank["rank"].min()
    rank = rank.to_dict()["rank"]
    for node in cyto_json["elements"]["nodes"]:
        name = caibg["objects"][int(node["data"]["id"])]["name"]
        zone = caibg["objects"][int(node["data"]["id"])]["zone"]
        url = caibg["objects"][int(node["data"]["id"])]["url"]
        value = rank[int(node["data"]["id"])]
        if ":" in name:
            node["data"]["name"] = name.split(":")[0].split(" ")[1]
            node["data"]["full_name"] = name.split(": ")[1]
        else:
            node["data"]["name"] = "".join([w[0] for w in name.split(" ")])
            node["data"]["full_name"] = name
        node["data"]["zone_color"] = color[zone]
        node["data"]["zone"] = zone
        node["data"]["value"] = value
        node["data"]["rank"] = str((value / min_rank).round(1))
        node["data"]["url"] = url
    cyto_str = json.dumps(cyto_json)
    cyto_str = re.sub(r'"source":\ (\d?\d?\d)', r'"source": "\1"', cyto_str)
    cyto_str = re.sub(r'"target":\ (\d?\d?\d)', r'"target": "\1"', cyto_str)
    return json.loads(cyto_str)


if __name__ == "__main__":
    try:
        with open("./data/caibg.json", "r") as fs:
            caibg = json.load(fs)
        rank = pd.read_csv("./data/caibg.csv", header=None, names=["rank"])
        dmap = {}
        for i, el in enumerate(caibg["objects"]):
            dmap[el["url"]] = i
        source = []
        target = []
        for i, el in enumerate(caibg["objects"]):
            for url in el["urls"]:
                if url not in dmap:
                    print(f"[WARNING] {url} in {el} not present in dmap")
                source.append(dmap[el["url"]])
                target.append(dmap[url])
        edges = pd.DataFrame({"source": source, "target": target})
        G = nx.from_pandas_edgelist(edges)
        cyto = nx.cytoscape_data(G)
        cyto_meta = _metadata(cyto, caibg, rank, cmap)
        with open("./cyto/data.json", "w") as fs:
            json.dump(cyto_meta, fs, indent=2)
        sys.exit(0)
    except Exception as err:
        print(f"[ERROR] {err}")
        sys.exit(1)
