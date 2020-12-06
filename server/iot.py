import argparse
import io
import os
import sys
import time

from google.api_core.exceptions import AlreadyExists
from google.cloud import iot_v1
from google.cloud import pubsub
from google.oauth2 import service_account
from google.protobuf import field_mask_pb2 as gp_field_mask
from googleapiclient import discovery
from googleapiclient.errors import HttpError

def send_command(
    service_account_json, project_id, cloud_region, registry_id, device_id, command
):
    """Send a command to a device."""
    print("Sending command to device")
    client = iot_v1.DeviceManagerClient()
    device_path = client.device_path(project_id, cloud_region, registry_id, device_id)

    data = command.encode("utf-8")

    return client.send_command_to_device(device_path, data)

