kill $(ps xa | grep "python main_client.py" | head -n1 | cut -b-6) ; kill $(ps xa | grep "python main_servidor.py" | head -n1 | cut -b-6)
