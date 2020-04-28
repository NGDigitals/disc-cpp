#include <iostream>
#include <boost/algorithm/string.hpp>

#include "constant.hpp"
#include "util/util.hpp"
#include "network/node.hpp"
#include "network/client.hpp"
#include "network/server.hpp"
#include "lib/cxxopts/cxxopts.hpp"

int main(int argc, char **argv){

    cxxopts::Options options("\n", "");

    try{
        options.add_options()
            ("c, client", "Client to use - cli or cls", cxxopts::value<std::string>()->implicit_value("cli"))
            ("n, new", "Configure new node", cxxopts::value<std::string>()->implicit_value("true"))
            ("l, label", "Name for node", cxxopts::value<std::string>())
            ("s, server", "Host for node", cxxopts::value<std::string>()/*->default_value("0.0.0.0")*/)
            ("p, port", "Port for node", cxxopts::value<int>()/*->default_value("5000")*/)
            ("o, option", "Operation option", cxxopts::value<int>())
            ("i, input", "Input for selected option", cxxopts::value<std::vector<std::string>>())
            ("h, help", "Print usage")
        ;
        auto aResult = options.parse(argc, argv);
        if (aResult.count("help")){
            std::cout << options.help() << std::endl;
            exit(0);
        }
        if (aResult.count("new")){
            std::string sName = aResult["label"].as<std::string>();
            if (!Util::IsAlphaNum(sName)){
                std::cout << "Invalid node name" << std::endl;
                exit(0);
            }
            const std::string sHost = aResult["server"].as<std::string>();
            const int nPort = aResult["port"].as<int>();
            if (!Node::isIPv4(sHost)){
                std::cout << "Invalid host" << std::endl;
                exit(0);
            }
            char cAddress[25];
            boost::to_upper(sName);
            sprintf(cAddress, "http://%s:%d", sHost.c_str(), nPort);
            Node nNode = Node(sName, cAddress, Util::RandomString(10));
            Server sServer (nNode);
        }else if (aResult.count("client") && aResult.count("label")){
            std::string sClient = aResult["client"].as<std::string>();
            std::string sName = aResult["label"].as<std::string>();
            Node nNode = Node::GetInstance(sName);
            if(nNode.GetName().empty()){
                std::cerr << "Node details not found" << std::endl;
                exit(0);
            }else{
                int nOption;
                if(sClient.compare("cli") == 0){
                    do{
                        std::cout << std::endl << "Select an option:" << std::endl;
                        for ( const auto &aPair : constant::CLIENT_OPTIONS_MAP ) {
                            std::cout << aPair.first << " - " << aPair.second << std::endl;
                        }
                        std::cout << std::endl;
                        std::cin >> nOption;
                        Client::HandleCLI(nNode, nOption);
                    }while(nOption != -1);
                }else{
                    Node nNode = Node::GetInstance(sName);
                    nOption = aResult["option"].as<int>();
                    std::vector<std::string> vInput = aResult["input"].as<std::vector<std::string>>();
                    Client::HandleCLS(nNode, nOption, vInput);
                }
            }
        }else{
            std::cout << options.help() << std::endl;
            exit(0);
        }
    }catch(std::domain_error &e){
        std::cout << options.help() << std::endl;
        exit(0);
    }catch(cxxopts::OptionSpecException &e){
        std::cout << options.help() << std::endl;
        exit(0);
    }catch(cxxopts::OptionParseException &e){
        std::cout << options.help() << std::endl;
        exit(0);
    }
    // }catch(cxxopts::option_not_exists_exception &e){
    //     std::cout << options.help() << std::endl;
    //     exit(0);
    // }catch(cxxopts::option_requires_argument_exception &e){
    //     std::cout << options.help() << std::endl;
    //     exit(0);
    // }
    return 0;
}