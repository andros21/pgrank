#!/usr/bin/env python
"""Digest json and csv data to create markdown table"""

import json
import sys

from pandas import read_csv

if __name__ == "__main__":
    try:
        df = read_csv("./data/caibg.csv", header=None, names=["pgrank"])
        df["score"] = (df["pgrank"] / df["pgrank"].min()).round(1)
        with open("./data/caibg.json", "r") as fs:
            caibg_dict = json.load(fs)
        name = [f"[{el['name']}]({el['url']})" for el in caibg_dict["objects"]]
        zone = [el["zone"] for el in caibg_dict["objects"]]
        df["name"] = name
        df["zone"] = zone
        df.sort_values(by="pgrank", ascending=False, inplace=True)
        df["pgrank"] = (df["pgrank"] * 100).round(2)
        df.to_markdown("./caibg.md", index=None)
        sys.exit(0)
    except Exception as err:
        print(f"[ERROR] {err}")
        sys.exit(1)
