print("IA Lua loaded")

function getNextMove(positionX, positionY)
    local trackDir = map[positionX][positionY]
    local nextMove = movements[trackDir]
    print(string.format("positionX: %d, positionY: %d, trackDir: %s,
    nextMove: %s", positionX, positionY, trackDir, nextMove))
    return nextMove
end