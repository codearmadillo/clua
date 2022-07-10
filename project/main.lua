local states = {};

-- New state
function createMenuState()
    local state = clua.state.create()

    state.setAsDefault();
    state.setAsActive();

    return state;
end

-- Clua hooks
function clua.start()
    states.menu = createMenuState()
    states.menu.destroy()
end
function clua.draw()
    -- Draw global stuff here, active state is drawn automatically
end
function clua.update(deltaTime)
    -- Do some global fun stuff here, active state is updated automatically
end