#include "srl.hpp"
#include "srl/Message.hpp"

#include "concurrent/Time.hpp"

#include <iostream>

using namespace std;
using namespace al;

class MyService : public srl::CallbackConnection
{
    protected:
        virtual void handle( const std::string message )
        {
            cout << "MyService::handle\n" << message << endl;
        }

};

int main( void )
{
    srl::Controller controller;

    // Establish available communication interfaces. Make them unmanaged.
    srl::CallbackInterface callback_interface;
    controller.register_interface(&callback_interface, false);

    // Start the controller.
    controller.start();

    // Create a connection.
    MyService service_connection;
    controller.add_connection(callback_interface.connect(service_connection));

    // Connect a new service.
    srl::RegisterServiceMessage services;
    services.set_services({"serviceA", "serviceB", "serviceC"});
    service_connection.send(services.encode());

    // Wait for message to be handled.
    while (service_connection.is_connected()) concurrent::msleep(1);

    // Stop the controller.
    cout << "Stopping controller..." << endl;
    controller.stop();
    controller.join();

    // Done.
    cout << "Done." << endl;

    /*
    string valid_json = "{\"name\": \"Anthony\", \"age\": 28, \"a1\": [\"jim\", 14, \"42\"], \"a2\": [1, 2, 3]}";
    cout << valid_json << endl;

    srl::Message message(valid_json);
    cout << "Read okay? " << (message.is_valid() ? "YES" : "NO") << endl;
    if (message.is_valid())
    {
        cout << "message['name']: " << message["name"].string() << endl;
        cout << "message['age'] : " << message["age"].integer() << endl;
        cout << "message['a1']  : [   ";
        for (int i = 0; i < message["a1"].size(); ++i)
        {
            if (message["a1"][i].is_integer()) cout << message["a1"][i].integer();
            else if (message["a1"][i].is_string()) cout << "'" << message["a1"][i].string() << "'";
            else cout << "???";
            cout << "   ";
        }
        cout << "]" << endl;
        cout << "message['a2']  : [   ";
        for (int i = 0; i < message["a2"].size(); ++i)
            cout << message["a2"][i].integer() << "   ";
        cout << "]" << endl;
    }
    else cout << message.get_last_error() << endl;

    // Add an object.
    message.create("my_new_int", 14);
    message.create("my_new_string", "test");
    message.create_array("a3");
    message["a3"][0] = 1;
    message["a3"][1] = "2";
    message["a3"][2] = 3;
    message["a3"][3] = 14.2;
    message.create_object("o1");
    message["o1"]["field"] = "test";
    message.create_object("o2");
    message.create_array("a4");

    cout << "The message encoded:" << endl;
    cout << message.encode() << endl;
    */

    return 0;
}