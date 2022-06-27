local states = {};

-- New state
function createMenuState()
    local state = clua.state.create()

    state.setAsActive()
    state.setAsDefault()

    function state.draw()
        -- Draw stuff here
    end
    function state.update(deltaTime)
        -- Do some fun stuff here
    end

    return state
end

-- Clua hooks
function clua.start()
    states.menu = createMenuState()
end
function clua.draw()
    -- Draw global stuff here, active state is drawn automatically
end
function clua.update(deltaTime)
    -- Do some global fun stuff here, active state is updated automatically
end