#!/usr/bin/env python3

import json
import sys

if len(sys.argv) < 2: exit(0)

def json_open(file_name:str) -> dict:
    with open(file_name, 'r') as f:
        lines = f.readlines();
        lines = ''.join(lines);
        return json.loads(lines)

def json_save(file_name:str, json_data:dict):
    with open(file_name, 'w') as f:
        f.write(json.dumps(json_data))

def tmj_layer_find(tmj_data:dict, name:str) -> dict:
    layers = tmj_data["layers"]
    for l in layers:
        if l["name"] == name:
            return l
    return {}

def tmj_object_property_get(tmj_obj:dict, name:str):
    if not "properties" in tmj_obj.keys(): return 0
    props = tmj_obj["properties"]
    for p in props:
        if p["name"] == name:
            return p["value"]
    return 0

def map_collision_from_tmj(col_layer:dict) -> [int]:
    dat = col_layer["data"]
    for i in range(len(dat)):
        dat[i]-=1
        if dat[i] == -1: dat[i] = 16
    return dat

def map_transition_from_tmj(tran_layer:dict) -> [dict]:
    dat = tran_layer["objects"]
    out = []
    for o in dat:
        co = {}
        co["x1"] = o["x"]
        co["y1"] = o["y"]
        co["x2"] = o["x"] + o["width"]
        co["y2"] = o["y"] + o["height"]
        co["dest"] = int(tmj_object_property_get(o, "destination"))
        dir = str(tmj_object_property_get(o, "direction"))
        flags = 0
        if (dir == "right"): flags |= 0
        if (dir == "left"): flags |= 1
        if (dir == "down"): flags |= 2
        if (dir == "up"): flags |= 3
        co["flags"] = flags
        out.append(co)

    return out

def translate_tileid_to_bloctype(tileid:int) -> int:
    if tileid == 257: return 0
    if tileid == 258: return 1
    if tileid == 259: return 6
    if tileid == 260: return 2
    if tileid == 277: return 3
    if tileid == 278: return 4
    if tileid == 279: return 5
    return 0

def map_bloc_from_tmj(bloc_layer:dict) -> [dict]:
    dat = bloc_layer["objects"]
    out = []
    for o in dat:
        co = {}
        co["x"] = int(o["x"]/24)
        co["y"] = int(o["y"]/24)
        co["t"] = translate_tileid_to_bloctype(o["gid"])
        out.append(co)
    return out

def tmj_process(tmj_file:str, map_file:str, name:str = "UNNAMED", x:int = 0, y:int = 0):
    tmj = json_open(tmj_file)
    print(tmj_file)
    out = {}
    out["name"] = name;
    out["width"] = tmj["width"];
    out["height"] = tmj["height"];
    out["x"] = x;
    out["y"] = y;

    collisions = tmj_layer_find(tmj, "collisions")
    if (collisions != {}): out["collision"] = map_collision_from_tmj(collisions)

    transitions =  tmj_layer_find(tmj, "transitions")
    if (transitions != {}): out["transitions"] = map_transition_from_tmj(transitions)

    blocs = tmj_layer_find(tmj, "blocs")
    if (blocs != {}): out["blocs"] = map_bloc_from_tmj(blocs)

    json_save(map_file, out)

def world_process(file_name:str):
    world = json_open(file_name)
    maps = world["maps"]
    folder_name = '/'.join(file_name.split("/")[:-1]) + '/'
    maps_name = []
    for m in maps:
        tmj_file = m["fileName"]
        map_file = tmj_file.split('.')[0] + ".map"
        maps_name.append(map_file)
        tmj_process(folder_name + tmj_file, folder_name + map_file, tmj_file, m["x"], -m["y"])
    with open(file_name.split('.')[0] + ".area", 'w') as f:
        f.writelines('\n'.join(maps_name))

world_process(sys.argv[1])
