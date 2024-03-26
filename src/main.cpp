#include <ge.hpp>

#include <iostream>

class PrintDelta : public ge::ent::Component {
public:
    void ready(ge::Ge& ge, ge::ent::Entity entity) override {

    }

    void update(ge::Ge& ge, ge::ent::Entity entity, double delta) override {

    }

    void input(ge::Ge& ge, ge::ent::Entity entity, const g_app::Event& event) override {
        switch(event.type){
            case g_app::EventType::KEY:
                if(event.key.key == g_app::Key::ESCAPE && event.key.action == g_app::Action::PRESS){
                    ge.quit();
                }
            default:
                break;
        }
    }
};

void spawn_print_delta(ge::Ge& ge){
    auto& ent = ge.ent();

    auto entity = ent.create_entity();
    ent.register_component<PrintDelta>();
    ent.spawn_component<PrintDelta>(entity, PrintDelta());
}

int main() {
    auto ge = ge::Ge({
        {800, 600},
    });
    ge.add_startup_system(spawn_print_delta);
    ge.run();

    return 0;
}
