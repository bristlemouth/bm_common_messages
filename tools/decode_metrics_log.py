import base64
import logging
import re
from pathlib import Path

import cbor2
from datetime import datetime, timezone

logger = logging.getLogger(__name__)

# Canonical metrics-log line + CBOR envelope decoder. Single source of truth for
# the wire format, living with the message schema. Consumers (spotter_hil HIL
# checks, bm_protocol debug CSV) layer their own output on top of these rows.
_LINE_RE = re.compile(
    r"^(?P<ts>\S+)\s*\|\s*(?P<node>\S+)/metrics\s+(?P<b64>[A-Za-z0-9+/=]+)"
)


def _parse_log_time(ts: str):
    """Parse the Spotter's leading log field.

    Returns (tick_ms, timestamp_utc):
      tick_ms       - ms; a boot-relative tick, or Unix epoch ms when the source
                      is a UTC timestamp.
      timestamp_utc - ISO-8601 UTC string when wall-clock is available, else None.
    """
    tick = ts.rstrip("t")
    if tick.isdigit():
        return int(tick), None
    try:
        dt = datetime.fromisoformat(ts.replace("Z", "+00:00"))
        return int(dt.timestamp() * 1000), dt.astimezone(timezone.utc).isoformat()
    except ValueError:
        return None, None


def decode_metrics_log(log_path: str | Path):
    """Yield one generic row per (line, component) from a network_metrics.log.

    Each row: {tick_ms, timestamp_utc, node_id, version, uptime_ms, component,
    fields}. Malformed/non-matching lines are skipped; missing file yields nothing.
    """
    try:
        with open(log_path, "r") as fd:
            lines = fd.readlines()
    except FileNotFoundError:
        logger.warning("metrics log not found: %s", log_path)
        return

    for line in lines:
        match = _LINE_RE.match(line)
        if not match:
            continue
        try:
            env = cbor2.loads(base64.b64decode(match.group("b64")))
        except Exception as e:  # malformed base64/CBOR - skip the line
            logger.debug("skipping undecodable metrics line: %s", e)
            continue
        tick_ms, timestamp_utc = _parse_log_time(match.group("ts"))
        node = env.get("node_id")
        node_id = f"{node:016x}" if isinstance(node, int) else match.group("node")

        data = env.get("data", {})
        if not isinstance(data, dict):
            continue
        for component, fields in data.items():
            if not isinstance(fields, dict):
                continue
            yield {
                "tick_ms": tick_ms,
                "timestamp_utc": timestamp_utc,
                "node_id": node_id,
                "version": env.get("version"),
                "uptime_ms": env.get("uptime_ms"),
                "component": component,
                "fields": dict(fields),
            }
