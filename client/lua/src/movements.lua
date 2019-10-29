version = "Beta"

movements = {
    ["horizontal"] = nil,
    ["vertical"] = nil,
    ["down-right"] = "right",
    ["down-left"] = "left",
    ["up-left"] = "left",
    ["up-right"] = "right",
}

map = {}
map[1] = {}
map[2] = {}
map[1][1] = "down-right"
map[1][2] = "up-right"
map[2][1] = "down-left"
map[2][2] = "up-left"
