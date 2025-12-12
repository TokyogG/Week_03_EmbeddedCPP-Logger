import edge_logger

edge_logger.init_logger()

value = edge_logger.read_sample()
print("Sample value:", value)

edge_logger.run_benchmark()
