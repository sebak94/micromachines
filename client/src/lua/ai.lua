
function getNextMove(positionX, positionY)
    local trackDir = map[positionX][positionY]
    local nextMove = movements[trackDir]
    print(string.format("positionX: %d, positionY: %d, trackDir: %s,
    nextMove: %s", positionX, positionY, trackDir, nextMove))
    return nextMove
end

track = {}

function init_load_map()
    for i = 1, 2 do
        track[i] = {}
        for j = 1, 2 do
            track[i][j] = foo[i][j];
        end
    end
end
