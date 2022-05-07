import math
import logging
from datetime import datetime

import torch
from torch.utils.data import DataLoader
from datasets import load_dataset
from sentence_transformers import SentenceTransformer,  LoggingHandler, losses, models, util
from sentence_transformers.evaluation import EmbeddingSimilarityEvaluator
from sentence_transformers.readers import InputExample

logging.basicConfig(
    format="%(asctime)s - %(message)s",
    datefmt="%Y-%m-%d %H:%M:%S",
    level=logging.INFO,
    handlers=[LoggingHandler()],
)
model_name = "klue/roberta-base"
train_batch_size = 16
num_epochs = 4
model_save_path = """C:/Users/이태관/PycharmProjects/chatbot_parctice/augmentaion/sentence_transformers/output/training_klue_sts_klue-roberta-base-2022-02-19_01-09-04"""
embedding_model = models.Transformer(model_name)
pooler = models.Pooling(
    embedding_model.get_word_embedding_dimension(),
    pooling_mode_mean_tokens=True,
    pooling_mode_cls_token=False,
    pooling_mode_max_tokens=False,
)

#model = SentenceTransformer(modules=[embedding_model, pooler])
model = SentenceTransformer(model_save_path)

def compare_sentence(docs,query):
    document_embeddings = docs
    query_embedding = query
    document_embeddings = model.encode(document_embeddings)
    query_embedding = model.encode(query_embedding)
    cos_scores = util.pytorch_cos_sim(query_embedding, document_embeddings)
    if float(cos_scores[0,0]) > 0.4:
        return True
    else:
        return False
